#ifndef WISHWIM_POW_H
#define WISHWIM_POW_H
#include "gui_entities.h"

#include "../../io_management/viewport_history.h"


bool gui_resumeCompletionTextAnchor(ViewPort* view_port, Cursor* cursor);

void gui_showCompletionTextAnchor(ViewPort* view_port, Cursor* cursor);

void gui_showCompletion(GUIContext* gui_context, int y, int x);

void gui_setLastTextAnchor(GUIContext* gui_context, CursorDescriptor descriptor);

void gui_showDiagnostic(GUIContext* gui_context, int y, int x, Diagnostic* diagnostic);

void gui_printPopup(EDW_GUIContext* context, Cursor* cursor, LSP_ComputedData* lsp_data);

bool gui_handlePopupInput(GUIContext* context, Cursor* cursor, int c_hash, int c_raw, LSP_ComputedData* lsp_data,
                          History** history_p, PayloadStateChange payload_state_change);

#endif // WISHWIM_POW_H
