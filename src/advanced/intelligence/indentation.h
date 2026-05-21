#ifndef INDENTATION_H
#define INDENTATION_H

#include "../../data-management/file_management.h"

void ilf_indentSelectedLines(FileContainer* fc, History** history_frame, PayloadStateChange* payload_state_change);
void ilf_deindentSelectedLines(FileContainer* fc, History** history_frame, PayloadStateChange* payload_state_change);

#endif // INDENTATION_H
