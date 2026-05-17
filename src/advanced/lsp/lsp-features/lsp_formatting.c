#include "lsp_formatting.h"
#include <stdlib.h>
#include <string.h>
#include "../../../environnement/constants.h"
#include "../../../environnement/global_variables.h"
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

void askOnTypeFormatting(FileContainer* file, char* ch, ModuleContext* data) {
  if (!file->lsp_datas.is_enable) {
    return;
  }
  LSP_Server* lsp = getLSPServerForLanguage(&lsp_servers, file->lsp_datas.lang_id);
  if (lsp == NULL) {
    return;
  }

  bool is_trigger = false;
  for (int i = 0; i < lsp->on_type_trigger_chars_count; i++) {
    if (strcmp(lsp->on_type_trigger_chars[i], ch) == 0) {
      is_trigger = true;
      break;
    }
  }

  if (is_trigger) {
    LSP_FormattingOptions options = {
      .tabSize = TAB_SIZE,
      .insertSpaces = !TAB_CHAR_USE,
      .trimTrailingWhitespace = true,
      .insertFinalNewline = true,
      .trimFinalNewlines = true,
    };

    LSP_requestOnTypeFormatting(lsp, file->io_file.path_abs,
                                LSP_pos(data->cursor->file_id.absolute_row - 1, data->cursor->line_id.absolute_column),
                                ch, options);
  }
}

void executeLspFormatting(Cursor* cursor, LSP_TextEdit* edits, int edits_size, History** history_p,
                          PayloadStateChange payload_state_change) {
  // Sort edits from bottom to top to avoid offset issues
  qsort(edits, edits_size, sizeof(LSP_TextEdit), compareTextEdit);

  // Tracker for the cursor position (0-based LSP)
  LSP_Position tracker = {.row = cursor->file_id.absolute_row - 1, .column = cursor->line_id.absolute_column};

  for (int i = 0; i < edits_size; i++) {
    LSP_Position edit_start = edits[i].range.pos1;
    LSP_Position edit_end = edits[i].range.pos2;
    LSP_Position new_text_end = calculateEndPos(edit_start, edits[i].new_text);

    // Apply the edit
    applyTextEdit(cursor, &edits[i], history_p, payload_state_change);

    // Adjust tracker
    if (compareLSPPos(tracker, edit_end) >= 0) {
      // Tracker is AFTER or AT the end of the original edit range
      if (tracker.row == edit_end.row) {
        // Tracker is on the same line as the end of the edit
        tracker.column = new_text_end.column + (tracker.column - edit_end.column);
      }
      tracker.row += (new_text_end.row - edit_end.row);
    }
    else if (compareLSPPos(tracker, edit_start) > 0) {
      // Tracker is INSIDE the edit range
      // Move tracker to the end of the new text
      tracker = new_text_end;
    }
    // Else: Tracker is BEFORE the edit, no shift needed
  }

  // Final restoration of the cursor
  *cursor = tryToReachAbsPosition(*cursor, tracker.row + 1, tracker.column);
}
