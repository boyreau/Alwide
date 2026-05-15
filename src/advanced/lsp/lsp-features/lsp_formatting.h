#ifndef LSP_FORMATTING_H
#define LSP_FORMATTING_H

#include "../lsp_dispatcher.h"
#include "../lsp_client.h"

void receiveFormattingData(cJSON* packet, FileContainer* file, ModuleContext* data);

void askFormatting(FileContainer* file);

void executeLspFormatting(Cursor* cursor, LSP_TextEdit* edits, int edits_size, History** history_p,
                            PayloadStateChange payload_state_change);

#endif // LSP_FORMATTING_H
