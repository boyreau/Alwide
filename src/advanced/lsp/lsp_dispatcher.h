#ifndef WISHWIM_LSP_DISPATCHER_H
#define WISHWIM_LSP_DISPATCHER_H

#include "../../../lib/cJSON/cJSON.h"

#include "../../data-management/file_management.h"

typedef struct {
  FileContainer* files;
  int size;
} DispatcherPayload;


void dispatcher(cJSON* packet, void* payload);

#endif // WISHWIM_LSP_DISPATCHER_H
