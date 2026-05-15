#ifndef ALWIDE_LSP_TOOLS_H
#define ALWIDE_LSP_TOOLS_H

#include "../../../data-management/file_structure.h"
#include "../../../data-management/state_control.h"
#include "../../shared.h"
#include "../lsp_client.h"


void applyTextEdit(Cursor* cursor, LSP_TextEdit* text_edit, History** history_p,
                   PayloadStateChange payload_state_change);

int compareTextEdit(const void* e1_p, const void* e2_p);


#endif //ALWIDE_LSP_TOOLS_H
