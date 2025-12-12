#include "lsp_response_dispatcher.h"

#include <string.h>

#include "../../terminal/windows/pow.h"

void receiveCompletionData(cJSON* packet, FileContainer* file) {
  LSP_destroyCompletionList(&file->lsp_datas.computed->completions);
  LSP_getCompletionListFromJSON(LSP_getPacketResult(packet), &file->lsp_datas.computed->completions);
}


void responseDispatcher(cJSON* packet, LSP_Server* lsp, DispatcherPayload* data) {
  LSP_PacketID id = LSP_getPacketID(packet);

  LSP_ResponseContext context;
  LSP_popResponseContext(lsp, id, &context);

  int index = getIndexFileContainerForName(data, context.file_name);
  if (index == -1) {
    fprintf(stderr, "ERROR : Couldn't find the file for the current response.\n");
    exit(-1); // TODO remove
    return;
  }

  if (cJSON_GetObjectItem(packet, "error")) {
    fprintf(stderr, "LSP : ERROR RECEIVED from %s !\n    => Method issue : %s.\n    => Error message : %s\n", lsp->language,
            context.method, cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(packet, "error"), "message")));
    return;
  }

  if (strcmp(context.method, "textDocument/completion") == 0) {
    // TODO implement the handle of the completion receive !
    fprintf(stderr, "RECEIVE completion !\n");
    receiveCompletionData(packet, data->files + index);
  }
  else {
    fprintf(stderr, "Response method NOT SUPPORTED !\n      => %s\n", context.method);
    exit(-1); // TODO remove
  }
}
