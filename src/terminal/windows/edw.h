#ifndef WISHWIM_FTW_H
#define WISHWIM_FTW_H
#include "../../data-management/file_management.h"
#include "../highlight.h"
#include "gui_entities.h"

void initEDWContext(EDW_GUIContext* context);

void resizeEDW(GUIContext* gui_context, int lnw_new_width);

void repaintEDW(EDW_GUIContext* context, Cursor cursor, Cursor select_cursor, int screen_x, int screen_y,
                WindowHighlightDescriptor* highlight_descriptor);

int getEDW_LengthLineNumber(GUIContext* gui_context);

#endif // WISHWIM_FTW_H
