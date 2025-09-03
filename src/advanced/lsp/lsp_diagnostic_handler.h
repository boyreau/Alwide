#ifndef WISHWIM_LSP_DIAGNOSTIC_HANDLER_H
#define WISHWIM_LSP_DIAGNOSTIC_HANDLER_H

#include "../../../lib/cJSON/cJSON.h"

void receiveDiagnostic(cJSON* packet, long* payload);


#endif // WISHWIM_LSP_DIAGNOSTIC_HANDLER_H
