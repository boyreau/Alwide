#include "lsp_formatting.h"
#include <stdlib.h>
#include <string.h>
#include "../../../environnement/constants.h"
#include "../../../environnement/global-variables.h"
#include "../../../terminal/windows/edw.h"
#include "../lsp_handler.h"
#include "lsp_completion.h"
#include "lsp_tools.h"

void receiveFormattingData(cJSON* packet, FileContainer* file, ModuleContext* data) {
  cJSON* result = cJSON_GetObjectItem(packet, "result");
  if (result == NULL || cJSON_IsNull(result)) {
    return;
  }

  int edits_size = cJSON_GetArraySize(result);
  if (edits_size == 0) {
    return;
  }

  LSP_TextEdit* edits = malloc(edits_size * sizeof(LSP_TextEdit));
  for (int i = 0; i < edits_size; i++) {
    edits[i] = LSP_getTextEditFromJSON(cJSON_GetArrayItem(result, i));
  }

  executeLspFormatting(data->cursor, edits, edits_size, &file->history_frame, data->payload_state_change);

  for (int i = 0; i < edits_size; i++) {
    LSP_destroyTextEdit(edits[i]);
  }
  free(edits);

  gui_updateEDW(data->view_port.gui);
}

void askFormatting(FileContainer* file) {
  if (!file->lsp_datas.is_enable) {
    return;
  }

  LSP_FormattingOptions options = {
    .tabSize = TAB_SIZE,
    .insertSpaces = !TAB_CHAR_USE,
    .trimTrailingWhitespace = true,
    .insertFinalNewline = true,
    .trimFinalNewlines = true,
  };

  LSP_Server* lsp = getLSPServerForLanguage(&lsp_servers, file->lsp_datas.lang_id);
  if (lsp == NULL) {
    return;
  }

  LSP_requestFormatting(lsp, file->io_file.path_abs, options);
}

void executeLspFormatting(Cursor* cursor, LSP_TextEdit* edits, int edits_size, History** history_p,
                           PayloadStateChange payload_state_change) {
  // Sort edits from bottom to top to avoid offset issues
  qsort(edits, edits_size, sizeof(LSP_TextEdit), compareTextEdit);

  CursorDescriptor pos_to_follow = cursor_to_desc(*cursor);

  for (int i = 0; i < edits_size; i++) {
    LSP_Position edit_pos = edits[i].range.pos1;
    LSP_Position edit_end = edits[i].range.pos2;

    // Jump to the start of the edit to get 'before' state
    *cursor = tryToReachAbsPosition(*cursor, edit_pos.row + 1, edit_pos.column);
    CursorDescriptor before = cursor_to_desc(*cursor);

    applyTextEdit(cursor, &edits[i], history_p, payload_state_change);

    // Position after applyTextEdit (at the end of the inserted text)
    CursorDescriptor after = cursor_to_desc(*cursor);

    int row_delta = after.row - before.row;

    // We only care if the edit was BEFORE or AT the position we follow
    if (edit_pos.row + 1 < pos_to_follow.row ||
        (edit_pos.row + 1 == pos_to_follow.row && edit_pos.column <= pos_to_follow.column)) {

      // If the edit was on the same line as the following position
      if (edit_pos.row + 1 == pos_to_follow.row) {
        // If the edit ended before or at the following position on the same line
        if (edit_end.row == edit_pos.row && edit_end.column <= pos_to_follow.column) {
          if (row_delta == 0) {
            pos_to_follow.column += (after.column - before.column);
          }
          else {
            // Edit made it multi-line. Our following position is now relative to the NEW last line of the edit.
            pos_to_follow.column = after.column + (pos_to_follow.column - edit_end.column);
          }
        }
        else if (edit_end.row > edit_pos.row ||
                 (edit_end.row == edit_pos.row && edit_end.column > pos_to_follow.column)) {
          // Edit overlaps with the following position.
          // In this case, we move the following position to the end of the edit.
          pos_to_follow.column = after.column;
        }
      }
      pos_to_follow.row += row_delta;
    }
  }

  // Restore cursor to the followed position
  *cursor = tryToReachAbsPosition(*cursor, pos_to_follow.row, pos_to_follow.column);
}
