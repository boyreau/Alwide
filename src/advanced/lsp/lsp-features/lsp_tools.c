#include "lsp_tools.h"

#include "../../../environnement/global-variables.h"
#include "../../../data-management/file_management.h"

void applyTextEdit(Cursor* cursor, LSP_TextEdit* text_edit, History** history_p,
                   PayloadStateChange payload_state_change) {
  // As a text edit can represent a "replacement" we have to handle this deleting old text and inserting new text after.
  // Delete part
  *cursor = LSP_tryToReachCursorForLSPPosition(*cursor, text_edit->range.pos1);
  Cursor end = LSP_tryToReachCursorForLSPPosition(*cursor, text_edit->range.pos2);
  deleteSelectionWithState(history_p, cursor, &end, payload_state_change);
  // insert part
  *cursor = insertCharArrayAtCursorWithHist(history_p, *cursor, text_edit->new_text, payload_state_change);
}

int compareTextEdit(const void* e1_p, const void* e2_p) {
  LSP_TextEdit* e1 = (LSP_TextEdit*)e1_p;
  LSP_TextEdit* e2 = (LSP_TextEdit*)e2_p;
  if (e1->range.pos1.row < e2->range.pos1.row)
    return 1;
  if (e1->range.pos1.row > e2->range.pos1.row)
    return -1;

  return e1->range.pos1.column <= e2->range.pos1.column ? 1 : -1;
}