#include "lsp_response_dispatcher.h"

#include <string.h>


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

  if (strcmp(context.method, "textDocument/completion") == 0) {
    // TODO implement the handle of the completion receive !
    fprintf(stderr, "RECEIVE completion !\n");
  }
  else {
    fprintf(stderr, "Response method NOT SUPPORTED !\n      => %s\n", context.method);
    exit(-1); // TODO remove
  }
}
