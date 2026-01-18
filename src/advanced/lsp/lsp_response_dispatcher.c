#include "lsp_response_dispatcher.h"

#include <string.h>

#include "../../terminal/windows/edw.h"
#include "../../terminal/windows/pow.h"

void receiveCompletionData(cJSON* packet, FileContainer* file, GUIContext* gui) {
  LSP_destroyCompletionList(&file->lsp_datas.computed->completions);
  LSP_getCompletionListFromJSON(LSP_getPacketResult(packet), &file->lsp_datas.computed->completions);
  if (file->lsp_datas.computed->completions.completions.size == 0) {
    if (gui->edw_context.pow_owner == COMPLETION) {
      gui_closePopup(gui);
    }
  }
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
    if (strcmp(context.method, "textDocument/completion") == 0) {
      if (data->gui->edw_context.pow_owner == COMPLETION) {
        gui_closePopup(data->gui);
      }
    }
    fprintf(stderr, "LSP : ERROR RECEIVED from %s !\n    => Method issue : %s.\n    => Error message : %s\n",
            lsp->language, context.method,
            cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(packet, "error"), "message")));
    return;
  }

  if (strcmp(context.method, "textDocument/completion") == 0) {
    // TODO implement the handle of the completion receive !
    fprintf(stderr, "RECEIVE completion !\n");
    receiveCompletionData(packet, data->files + index, data->gui);
  }
  else {
    fprintf(stderr, "Response method NOT SUPPORTED !\n      => %s\n", context.method);
    exit(-1); // TODO remove
  }
}
