#include "lsp_completion.h"

#include <assert.h>

#include "../../../data-management/file_management.h"
#include "../../../data-management/state_control.h"


void applyTextEdit(Cursor* cursor, TextEdit* text_edit, History** history_p, PayloadStateChange payload_state_change) {
  // As a text edit can represent a "replacement" we have to handle this deleting old text and inserting new text after.
  // Delete part
  *cursor = tryToReachAbsPosition(*cursor, text_edit->range.pos1.row + 1, text_edit->range.pos1.column);
  Cursor end = tryToReachAbsPosition(*cursor, text_edit->range.pos2.row + 1, text_edit->range.pos2.column);
  deleteSelectionWithState(history_p, cursor, &end, payload_state_change);
  // insert part
  *cursor = insertCharArrayAtCursorWithHist(history_p, *cursor, text_edit->new_text, payload_state_change);
}

int compareTextEdit(const void* e1_p, const void* e2_p) {
  TextEdit* e1 = (TextEdit*)e1_p;
  TextEdit* e2 = (TextEdit*)e2_p;
  if (e1->range.pos1.row < e2->range.pos1.row)
    return 1;
  if (e1->range.pos1.row > e2->range.pos1.row)
    return -1;

  return e1->range.pos1.column <= e2->range.pos1.column ? 1 : -1;
}

void LSP_executeCompletion(Cursor* cursor, CompletionItem* item, History** history_p,
                           PayloadStateChange payload_state_change) {
  // TODO handle the case of an lsp_server not using text-edit.
  // Create a text edit from the insertText.
  // if (item->insertText)

  bool main_text_edit_done = !item->is_text_edit;
  CursorDescriptor position_after_insert;
  position_after_insert.row = -1;
  position_after_insert.column = -1;

  // TODO may check if it check well using multiple additionalTextEdit
  qsort(item->additionalTextEdits, item->additionalTextEditsSize, sizeof(TextEdit), compareTextEdit);

  int i = 0;
  while (i < item->additionalTextEditsSize) {
    if (!main_text_edit_done && compareTextEdit(&item->text_edit, item->additionalTextEdits + i) != 1) {
      main_text_edit_done = true;
      applyTextEdit(cursor, &item->text_edit, history_p, payload_state_change);
      position_after_insert = cursorToDescriptor(cursor);
    }
    else {
      *cursor = tryToReachAbsPosition(*cursor, item->additionalTextEdits[i].range.pos1.row + 1,
                                      item->additionalTextEdits[i].range.pos1.column);
      CursorDescriptor tmp = cursorToDescriptor(cursor);
      applyTextEdit(cursor, item->additionalTextEdits + i, history_p, payload_state_change);
      i++;
      // if the end position of the cursor is already setted we have to follow the new add in the file to follow lines.
      // !! ISSUE !! we don't manage the follow if some column are added before in the same line.
      // We don't manage if the additionnal text edit is removing a lot of line.
      if (position_after_insert.row != -1) {
        position_after_insert.row += cursor->file_id.absolute_row - tmp.row;
      }
    }
  }

  if (!main_text_edit_done) {
    applyTextEdit(cursor, &item->text_edit, history_p, payload_state_change);
  }
  else if (position_after_insert.row != -1) {
    *cursor = tryToReachAbsPosition(*cursor, position_after_insert.row, position_after_insert.column);
  }
}
