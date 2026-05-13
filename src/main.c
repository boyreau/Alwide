#include <assert.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "advanced/lsp/lsp_client.h"
#include "advanced/lsp/lsp_dispatcher.h"
#include "advanced/lsp/lsp_highlighter.h"
#include "advanced/tree-sitter/tree_manager.h"
#include "config/config.h"
#include "data-management/file_management.h"
#include "data-management/state_control.h"
#include "environnement/constants.h"
#include "environnement/global-variables.h"
#include "io_management/io_explorer.h"
#include "io_management/workspace_settings.h"
#include "terminal/highlight.h"
#include "terminal/term_handler.h"
#include "utils/key_management.h"

#include "core/editor_context.h"
#include "core/editor_input.h"
#include "core/editor_lsp.h"
#include "core/editor_render.h"
#include "core/editor_state.h"
#include "environnement/setup.h"

// Global vars.
int color_pair = 5;
int color_index = 20;
cJSON* config;
ParserList parsers;
LSPServerLinkedList lsp_servers;
WorkspaceSettings workspace_settings;


int main(int file_count, char** args) {
  setup_environment();

  char** file_names = args;
  file_names++;
  file_count--;

  config = loadConfig();
  initParserList(&parsers);
  initLSPServerList(&lsp_servers);

  EditorContext ctx;
  ctx.file_count = file_count;
  ctx.current_file_index = 0;
  ctx.refresh_local_vars = true;
  ctx.old_history_frame = NULL;
  ctx.mouse_drag = false;
  ctx.last_time_mouse_drag = timeInMilliseconds();
  ctx.t_date = timeInMilliseconds();
  ctx.t_clock = clock();
  ctx.peek_c = -1;

  gui_initGUIContext(&ctx.gui_context);
  gui_initNCurses(&ctx.gui_context);

  setupWorkspace(&workspace_settings, &ctx.file_count, &file_names, &ctx.gui_context, &ctx.current_file_index);
  setupOpenedFiles(&ctx.file_count, file_names, &ctx.files);

  char* dir_path = getenv("PWD");
  if (dir_path == NULL) {
    dir_path = getenv("HOME");
  }
  initFolder(workspace_settings.is_used == true ? workspace_settings.dir_path : dir_path, &ctx.pwd);
  ctx.pwd.open = true;

  whd_init(&ctx.highlight_descriptor);

  while (true) {
    run_post_processing(&ctx);
    paint_gui(&ctx);

  read_input:;
    int c, hash;
    read_next_input(&ctx, &c, &hash);

    DispatcherPayload payload = build_dispatcher_payload(&ctx);
    dispatch_lsp(&payload, &c, &hash);

    // lsp ask to refresh local_vars
    if (ctx.refresh_local_vars) {
      // TODO careful we may drop an input doing this jump.
      continue;
    }

    // if no input was fetched
    if (hash == ERR) {
      goto read_input;
    }

    if (hash == KEY_MOUSE) {
      if (getmouse(&ctx.m_event) != OK) {
        fprintf(stderr, "MOUVE_EVENT_NOT_OK !\r\n");
        goto read_input;
      }
      detectComplexMouseEvents(&ctx.m_event);
    }

    EventLoopAction action = EVENT_CONTINUE;
    bool has_popup_handle_input = handle_popup_input(&ctx, c, hash, &payload);

    if (!has_popup_handle_input) {
      action = process_key_event(&ctx, c, hash);
    }


    // end while jump
    switch (action) {
      case EVENT_QUIT:
        goto end;
      case EVENT_READ_INPUT:
        if (gui_doesGUINeedRepaint(&ctx.gui_context)) {
          continue;
        }
        goto read_input;
      case EVENT_CONTINUE:
        continue;
      default:
        assert(false);
    }
  }

end:
  printf("\033[?1003l\033[0 q\n");
  fflush(stdout);

  whd_free(&ctx.highlight_descriptor);

  if (workspace_settings.is_used == true) {
    WorkspaceSettings new_settings;
    getWorkspaceSettingsForCurrentDir(&new_settings, ctx.files, ctx.file_count, ctx.current_file_index,
                                      ctx.gui_context.ofw_context.ofw_height != 0,
                                      ctx.gui_context.few_context.few_width != 0, FILE_EXPLORER_WIDTH);
    saveWorkspaceSettings(workspace_settings.dir_path, &new_settings);
    destroyWorkspaceSettings(&new_settings);
  }

  for (int i = 0; i < ctx.file_count; i++) {
    destroyFileContainer(ctx.files + i);
  }
  destroyFolder(&ctx.pwd);
  free(ctx.files);
  cJSON_Delete(config);
  destroyParserList(&parsers);

  usleep(30000);
  flushinp();
  endwin();
  return 0;
}
