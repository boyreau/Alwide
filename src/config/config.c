#include "config.h"

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool configExist() {
  char path[PATH_MAX];
  const char* home = getenv("HOME");
  if (!home) {
    return false;
  }
  snprintf(path, PATH_MAX, "%s/%s", home, CONFIG_PATH);

  FILE* f = fopen(path, "r");
  if (f == NULL) {
    return false;
  }
  fclose(f);
  return true;
}

void touchConfig() {
  char command[PATH_MAX + 100];
  const char* home = getenv("HOME");
  if (!home) {
    return;
  }
  snprintf(command, sizeof(command), "mkdir -p \"%s/%s\"", home, CONFIG_FOLDER);
  system(command);
  snprintf(command, sizeof(command), "touch \"%s/%s\"", home, CONFIG_PATH);
  system(command);
}

cJSON* loadConfig() {
  const char* home = getenv("HOME");
  if (!home) {
    return NULL;
  }

  if (configExist() == false) {
    touchConfig();

    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "%s/%s", home, CONFIG_PATH);
    fprintf(stderr, "Generating default config to %s\n", path);

    FILE* f = fopen(path, "w");
    if (f == NULL) {
      fprintf(stderr, "ERROR opening config file.\n");
      return NULL;
    }
    fprintf(f, DEFAULT_CONFIG);
    fclose(f);
  }

  char path[PATH_MAX];
  snprintf(path, PATH_MAX, "%s/%s", home, CONFIG_PATH);

  FILE* f = fopen(path, "rb");
  if (f == NULL) {
    return NULL;
  }

  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET); /* same as rewind(f); */

  if (fsize < 0) {
    fclose(f);
    return NULL;
  }

  char* file_content = malloc(fsize + 1);
  if (!file_content) {
    fclose(f);
    return NULL;
  }

  size_t bytes_read = fread(file_content, 1, fsize, f);
  fclose(f);

  file_content[bytes_read] = 0;

  cJSON* json = cJSON_Parse(file_content);

  free(file_content);
  return json;
}
