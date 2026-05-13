#ifndef WISHWIM_EDITOR_LSP_H
#define WISHWIM_EDITOR_LSP_H

#include "../advanced/lsp/lsp_dispatcher.h"
#include "editor_context.h"

ModuleContext buildModuleContext(EditorContext* ctx);
void handleLspServers(ModuleContext* payload, int* c, int* hash);

#endif
