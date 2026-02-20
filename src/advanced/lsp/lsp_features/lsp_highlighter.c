#include "lsp_highlighter.h"

#include <assert.h>
#include <ncurses.h>

#include "../../../terminal/highlight.h"
#include "../lsp_handler.h"


void LSP_highlightCurrentFile(LSP_Data* lsp_datas, Cursor cursor, WindowHighlightDescriptor* highlight_descriptor) {
  if (!lsp_datas->is_enable) {
    return;
  }
  assert(lsp_datas->computed != NULL);
  for (int i = 0; i < lsp_datas->computed->diagnostics_size; i++) {
    Cursor begin_cursor = tryToReachAbsPosition(cursor, lsp_datas->computed->diagnostics[i].range.pos1.row + 1,
                                                lsp_datas->computed->diagnostics[i].range.pos1.column + 1);
    Cursor end_cursor = tryToReachAbsPosition(cursor, lsp_datas->computed->diagnostics[i].range.pos2.row + 1,
                                              lsp_datas->computed->diagnostics[i].range.pos2.column);

    attr_t attr = A_UNDERLINE;
    NCURSES_PAIRS_T color = COLOR_RED;

    whd_insertDescriptor(highlight_descriptor, begin_cursor, end_cursor, color, attr, 0, false,
                         (LineMarker)lsp_datas->computed->diagnostics[i].severity,
                         lsp_datas->computed->diagnostics + i);
  }
}
