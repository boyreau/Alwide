#ifndef WISHWIM_OFW_H
#define WISHWIM_OFW_H
#include "../../data-management/file_management.h"
#include "gui_entities.h"

void initOFWContext(OFW_GUIContext* context);

void resizeOFW(GUIContext* gui_context);

void repaintOFW(OFW_GUIContext* context, FileContainer* files, int file_count, int current_file);

void switchOFW(GUIContext* gui_context);

#endif // WISHWIM_OFW_H
