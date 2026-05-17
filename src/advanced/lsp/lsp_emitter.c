#include "lsp_emitter.h"

#include "../../environnement/global_variables.h"

int global_version = 2;

void onStateChangeLSP(Action action, LSP_Data* data, Cursor* cursor) {
  if (!data->is_enable) {
    return;
  }
  LSP_Server* lsp_server = getLSPServerForLanguage(&lsp_servers, data->lang_id);
  if (lsp_server == NULL) {
    return;
  }

  ChangeDescriptor action_change = actionToChangeDescriptor(action);

  // Create array
  cJSON* array_of_changes = cJSON_CreateArray();

  // Create the object that the array will contain, describe de action_change
  cJSON* change = cJSON_CreateObject();
  // Add the 'range' field to the change
  LSP_Range lsp_range = LSP_range(LSP_pos(action_change.start_point.row, action_change.start_point.column),
                                  LSP_pos(action_change.old_end_point.row, action_change.old_end_point.column));
  cJSON* range = LSP_getJSONRange(lsp_range);
  cJSON_AddItemToObject(change, "range", range);
  // Add the 'text' field to the change
  Cursor begin =
    tryToReachAbsPosition(*cursor, LSP_0_row_to_1_row(action_change.start_point.row), action_change.start_point.column);
  Cursor end = tryToReachAbsPosition(*cursor, LSP_0_row_to_1_row(action_change.new_end_point.row),
                                     action_change.new_end_point.column);
  char* new_text = dumpSelection(begin, end);
  cJSON_AddStringToObject(change, "text", new_text);
  free(new_text);

  // Add the change to the array
  cJSON_AddItemToArray(array_of_changes, change);

  // Send the changes to the lsp server
  LSP_notifyLspFileDidChange(lsp_server, data->path_abs, array_of_changes, global_version++);
}
