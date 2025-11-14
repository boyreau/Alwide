#ifndef WISHWIM_LSP_DIAGNOSTIC_HANDLER_H
#define WISHWIM_LSP_DIAGNOSTIC_HANDLER_H

#include "../../../lib/cJSON/cJSON.h"
#include "lsp_dispatcher.h"


void notificationDispatcher(cJSON* packet, DispatcherPayload* data);

#endif // WISHWIM_LSP_DIAGNOSTIC_HANDLER_H
