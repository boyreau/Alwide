#include "pow.h"

#include <string.h>

#include "../../advanced/lsp/lsp_features/lsp_completion.h"
#include "../../utils/key_management.h"
#include "../term_handler.h"
#include "edw.h"


void gui_showCompletion(GUIContext* gui_context, int y, int x) {
  int height = min(getmaxy(gui_context->edw_context.ftw) - y, 7);
  int width = min(getmaxx(gui_context->edw_context.ftw) - x, 50);

  bool isOpened = gui_showPopup(gui_context, y + height, x - 2, height, width, COMPLETION);
  // couldn't show popup
  if (!isOpened) {
    return;
  }

  gui_context->edw_context.completion_offset_y = 0;
  gui_context->edw_context.completion_selected = 0;

  wbkgd(gui_context->edw_context.pow, COLOR_PAIR(INFO_COLOR_PAIR));
}

void gui_showDiagnostic(GUIContext* gui_context, int y, int x, Diagnostic* diagnostic) {
  if (diagnostic == NULL) {
    return;
  }

  int height = 3;
  int unused = 0;
  int max_width = getmaxx(gui_context->edw_context.ftw) - 2;

  int ch_length = strlen(diagnostic->message);
  countStringFrame(diagnostic->message, ch_length, &height, &unused, &max_width);

  bool isOpened = gui_showPopup(gui_context, y + height, x - 2, height,
                                min(getmaxx(gui_context->edw_context.ftw), max_width + 2), DIAGNOSTICS);
  // couldn't show popup
  if (!isOpened) {
    return;
  }

  int color = 0;
  switch (diagnostic->severity) {
    case LSP_ERROR:
      color = ERROR_COLOR_PAIR;
      break;
    case LSP_WARNING:
      color = WARNING_COLOR_PAIR;
      break;
    case LSP_INFORMATION:
      color = INFO_COLOR_PAIR;
      break;
    default:
      color = 0;
      break;
  }
  wbkgd(gui_context->edw_context.pow, COLOR_PAIR(color));
  wattr_set(gui_context->edw_context.pow, A_NORMAL, color, NULL);

  box(gui_context->edw_context.pow, 0, 0);

  printToWindow(gui_context->edw_context.pow, diagnostic->message, ch_length, 1, 1, max_width, 0);
}

void gui_printCompletionPopup(EDW_GUIContext* context, Cursor* cursor, LSP_ComputedData* lsp_data) {
  int width = getmaxx(context->pow), height = getmaxy(context->pow);
  werase(context->pow);

  // for the height lines.
  for (int i = 0; i < height; i++) {
    int index = context->completion_offset_y + i;
    if (index >= lsp_data->completions.completions.size) {
      break; // reach the end of the completions.
    }

    // setup color for current line.
    if (context->completion_selected == index) {
      wattr_set(context->pow, A_NORMAL, WARNING_COLOR_HOVER_PAIR, NULL);
    }
    else {
      wattr_set(context->pow, A_NORMAL, INFO_COLOR_PAIR, NULL);
    }

    printToWindow(context->pow, trim(lsp_data->completions.completions.items[index].label), -1, 0, i, width, 1);
  }
}


void gui_printPopup(EDW_GUIContext* context, Cursor* cursor, LSP_ComputedData* lsp_data) {
  switch (context->pow_owner) {
    case COMPLETION:
      gui_printCompletionPopup(context, cursor, lsp_data);
      break;
    default:
      break;
  }
}


bool gui_handleCompletionInput(GUIContext* context, Cursor* cursor, int c_hash, int c_raw, LSP_ComputedData* lsp_data,
                               History** history_p, PayloadStateChange payload_state_change) {
  int height = getmaxy(context->edw_context.pow);
  switch (c_hash) {
    case H_KEY_UP:
      if (context->edw_context.completion_selected > 0) {
        context->edw_context.completion_selected--;
      }
      if (context->edw_context.completion_selected < context->edw_context.completion_offset_y) {
        context->edw_context.completion_offset_y = context->edw_context.completion_selected;
      }
      return true;
    case H_KEY_DOWN:
      context->edw_context.completion_selected++;
      if (context->edw_context.completion_selected >= lsp_data->completions.completions.size) {
        context->edw_context.completion_selected = lsp_data->completions.completions.size - 1;
      }
      if (context->edw_context.completion_selected >= context->edw_context.completion_offset_y + height) {
        context->edw_context.completion_offset_y++;
      }
      return true;
    case '\n':
    case KEY_ENTER:
      LSP_executeCompletion(cursor, lsp_data->completions.completions.items + context->edw_context.completion_selected,
                            history_p, payload_state_change);
      gui_closePopup(context);
      return true;
    case KEY_TAB:
      LSP_executeCompletion(cursor, lsp_data->completions.completions.items + context->edw_context.completion_selected,
                            history_p, payload_state_change);
      gui_closePopup(context);
      return true;
    default:
      break;
  }

  return false;
}

bool gui_handlePopupInput(GUIContext* context, Cursor* cursor, int c_hash, int c_raw, LSP_ComputedData* lsp_data,
                          History** history_p, PayloadStateChange payload_state_change) {
  switch (context->edw_context.pow_owner) {
    case COMPLETION:
      return gui_handleCompletionInput(context, cursor, c_hash, c_raw, lsp_data, history_p, payload_state_change);
    default:
      break;
  }
  return false;
}
