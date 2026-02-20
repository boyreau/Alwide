#ifndef WISHWIM_LSP_COMPLETION_H
#define WISHWIM_LSP_COMPLETION_H

#include "../../../data-management/file_structure.h"
#include "../../../data-management/state_control.h"
#include "../../shared.h"
#include "../lsp_client.h"
#include "../../../data-management/file_management.h"
#include "../../../terminal/term_handler.h"

void LSP_executeCompletion(Cursor* cursor, CompletionItem* item, History** history_p,
                           PayloadStateChange payload_state_change);

void askCompletion(GUIContext* gui_context, Cursor* cursor, int* screen_x, int* screen_y, LSP_Data* lsp_data,
                   bool reset, bool force) ;


#endif // WISHWIM_LSP_COMPLETION_H
