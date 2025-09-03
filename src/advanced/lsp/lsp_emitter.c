#include "lsp_emitter.h"

#include <assert.h>

#include "../../utils/global-variables.h"

int global_version = 2;

void onStateChangeLSP(Action action, LSP_Data* data) {
  if (!data->is_enable) {
    return;
  }
  LSP_Server* lsp_server = getLSPServerForLanguage(&lsp_servers, data->lang_id);
  if (lsp_server == NULL) {
    return;
  }

  TSInputEdit edit;
  switch (action.action) {
    case INSERT:
      // system("echo \"=== INSERT ===\" >> tree_logs.txt");
      assert(action.byte_start != -1);
      assert(action.byte_end != -1);
      edit.start_byte = action.byte_start;
      edit.start_point.row = action.cur.file_id.absolute_row - 1;
      edit.start_point.column = action.cur.line_id.absolute_column;

      edit.old_end_byte = action.byte_start;
      edit.old_end_point.row = edit.start_point.row;
      edit.old_end_point.column = edit.start_point.column;

      edit.new_end_byte = action.byte_end;
      edit.new_end_point.row = action.cur_end.file_id.absolute_row - 1;
      edit.new_end_point.column = action.cur_end.line_id.absolute_column;
      // To force the match with previous node.
      break;
    case DELETE:
      // system("echo \"=== DELETE ===\" >> tree_logs.txt");
      assert(action.byte_start != -1);
      edit.start_byte = action.byte_start;
      edit.start_point.row = action.cur.file_id.absolute_row - 1;
      edit.start_point.column = action.cur.line_id.absolute_column;

      edit.old_end_byte = action.byte_end;

      // TODO may optimize
      // CALCULATE ROW AND COLUMN POINT
      char* ch = action.ch;
      int current_row = edit.start_point.row;
      int current_column = edit.start_point.column;

      int current_ch_index = 0;
      while (current_ch_index < action.byte_end - action.byte_start) {
        if (TAB_CHAR_USE == false) {
          assert(ch[current_ch_index] != '\t');
        }
        if (ch[current_ch_index] == '\n') {
          current_row++;
          current_column = 0;
        }
        else {
          Char_U8 tmp_ch = readChar_U8FromCharArray(ch + current_ch_index);
          current_ch_index += sizeChar_U8(tmp_ch) - 1;
          current_column++;
        }
        current_ch_index++;
      }

      edit.old_end_point.row = current_row;
      edit.old_end_point.column = current_column;


      edit.new_end_byte = action.byte_start;
      edit.new_end_point.row = edit.start_point.row;
      edit.new_end_point.column = edit.start_point.column;
      // To force the match with previous node.
      break;
    case DELETE_ONE:
      // system("echo \"=== DELETE_ONE ===\" >> tree_logs.txt");
      assert(action.byte_start != -1);
      edit.start_byte = action.byte_start;
      edit.start_point.row = action.cur.file_id.absolute_row - 1;
      edit.start_point.column = action.cur.line_id.absolute_column;

      edit.old_end_byte = action.byte_start + 1;
      edit.old_end_point.row = edit.start_point.row;
      edit.old_end_point.column = edit.start_point.column;
      if (action.unique_ch == '\n') {
        edit.old_end_point.row++;
        edit.old_end_point.column = 0;
      }
      else {
        edit.old_end_point.column++;
      }

      edit.new_end_byte = action.byte_start;
      edit.new_end_point.row = edit.start_point.row;
      edit.new_end_point.column = edit.start_point.column;
      // To force the match with previous node.

      break;
    default:
      assert(action.action == ACTION_NONE);
      return;
  }

  cJSON* array_of_changes = cJSON_CreateArray();

  cJSON* change = cJSON_CreateObject();

  cJSON* range =
    LSP_getJSONRange(edit.start_point.row + 1, edit.start_point.column, edit.old_end_point.row + 1, edit.old_end_point.column);

  cJSON_AddItemToObject(change, "range", range);


  char* new_text =
    dumpSelection(action.cur, tryToReachAbsPosition(action.cur, edit.new_end_point.row + 1, edit.new_end_point.column));

  cJSON_AddStringToObject(change, "text", new_text);

  free(new_text);

  cJSON_AddItemToArray(array_of_changes, change);

  LSP_notifyLspFileDidChange(*lsp_server, data->path_abs, array_of_changes, global_version++);
}
