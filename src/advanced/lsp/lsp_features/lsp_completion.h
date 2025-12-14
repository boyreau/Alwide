#ifndef WISHWIM_LSP_COMPLETION_H
#define WISHWIM_LSP_COMPLETION_H

#include "../../../data-management/file_structure.h"
#include "../../../data-management/state_control.h"
#include "../../shared.h"
#include "../lsp_client.h"

void LSP_executeCompletion(Cursor* cursor, CompletionItem* item, History** history_p,
                           PayloadStateChange payload_state_change);


#endif // WISHWIM_LSP_COMPLETION_H
