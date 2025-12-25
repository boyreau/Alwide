#ifndef WISHWIM_LSP_DISPATCHER_H
#define WISHWIM_LSP_DISPATCHER_H

#include "../../../lib/cJSON/cJSON.h"

#include "../../data-management/file_management.h"
#include "../../terminal/windows/gui_entities.h"

typedef struct {
  FileContainer* files;
  int size;
  GUIContext *gui;
} DispatcherPayload;


void dispatcher(cJSON* packet, LSP_Server* lsp, void* payload);

int getIndexFileContainerForUri(DispatcherPayload* payload, cJSON* params);

int getIndexFileContainerForName(DispatcherPayload* payload, char* file_name);

void printPacket(cJSON* packet, cJSON* params);

#endif // WISHWIM_LSP_DISPATCHER_H
