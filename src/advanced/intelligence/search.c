#include "search.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

static char* ilj_strcasestr(const char* haystack, const char* needle) {
  if (!*needle) return (char*)haystack;
  for (; *haystack; haystack++) {
    if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
      const char *h, *n;
      for (h = haystack, n = needle; *h && *n; h++, n++) {
        if (tolower((unsigned char)*h) != tolower((unsigned char)*n)) {
          break;
        }
      }
      if (!*n) return (char*)haystack;
    }
  }
  return NULL;
}

static int ilj_byteOffsetToCharOffset(const char* str, int byte_offset) {
  int char_offset = 0;
  for (int i = 0; i < byte_offset; i++) {
    if ((str[i] & 0xC0) != 0x80) {
      char_offset++;
    }
  }
  return char_offset;
}

static int ilj_byteCountToCharCount(const char* str) {
  int char_count = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if ((str[i] & 0xC0) != 0x80) {
      char_count++;
    }
  }
  return char_count;
}

static char* ilj_getLineString(LineNode* line) {
  int total_bytes = 0;
  LineNode* curr = line;
  while (curr != NULL) {
    total_bytes += curr->byte_count;
    curr = curr->next;
  }

  char* buf = malloc(total_bytes + 1);
  if (!buf) return NULL;

  int offset = 0;
  curr = line;
  while (curr != NULL) {
    for (int i = 0; i < curr->element_number; i++) {
      Char_U8 ch = curr->ch[i];
      int sz = sizeChar_U8(ch);
      for (int b = 0; b < sz; b++) {
        buf[offset++] = ch.t[b];
      }
    }
    curr = curr->next;
  }
  buf[offset] = '\0';
  return buf;
}

bool ilj_findNext(FileContainer* fc, const char* query, bool case_sensitive, bool wrap,
                  Cursor* start_cursor, Cursor* end_cursor) {
  if (!query || *query == '\0') {
    return false;
  }

  Cursor end_finder = tryToReachAbsPosition(fc->cursor, INT_MAX, INT_MAX);
  int total_lines = cursor_row(end_finder);
  if (total_lines <= 0) {
    return false;
  }

  int start_row = cursor_row(fc->cursor);
  int start_col = cursor_col(fc->cursor);

  int current_row = start_row;
  bool first_line = true;
  int query_char_len = ilj_byteCountToCharCount(query);

  while (true) {
    Cursor temp_cursor = tryToReachAbsPosition(fc->cursor, current_row, 0);
    char* line_str = ilj_getLineString(temp_cursor.line_id.line);
    if (!line_str) {
      return false;
    }

    const char* match = NULL;
    const char* scan_ptr = line_str;

    while (true) {
      if (case_sensitive) {
        match = strstr(scan_ptr, query);
      } else {
        match = ilj_strcasestr(scan_ptr, query);
      }
      if (!match) {
        break;
      }

      int byte_offset = match - line_str;
      int char_offset = ilj_byteOffsetToCharOffset(line_str, byte_offset);

      if (first_line && char_offset < start_col) {
        scan_ptr = match + 1;
        continue;
      }

      *start_cursor = tryToReachAbsPosition(fc->cursor, current_row, char_offset);
      *end_cursor = tryToReachAbsPosition(fc->cursor, current_row, char_offset + query_char_len);
      free(line_str);
      return true;
    }

    free(line_str);

    current_row++;
    first_line = false;

    if (current_row > total_lines) {
      if (wrap) {
        current_row = 1;
      } else {
        break;
      }
    }

    if (current_row == start_row && !first_line) {
      temp_cursor = tryToReachAbsPosition(fc->cursor, start_row, 0);
      line_str = ilj_getLineString(temp_cursor.line_id.line);
      if (line_str) {
        scan_ptr = line_str;
        while (true) {
          if (case_sensitive) {
            match = strstr(scan_ptr, query);
          } else {
            match = ilj_strcasestr(scan_ptr, query);
          }
          if (!match) {
            break;
          }

          int byte_offset = match - line_str;
          int char_offset = ilj_byteOffsetToCharOffset(line_str, byte_offset);

          if (char_offset >= start_col) {
            break;
          }

          *start_cursor = tryToReachAbsPosition(fc->cursor, start_row, char_offset);
          *end_cursor = tryToReachAbsPosition(fc->cursor, start_row, char_offset + query_char_len);
          free(line_str);
          return true;
        }
        free(line_str);
      }
      break;
    }
  }

  return false;
}

bool ilj_findPrev(FileContainer* fc, const char* query, bool case_sensitive, bool wrap,
                  Cursor* start_cursor, Cursor* end_cursor) {
  if (!query || *query == '\0') {
    return false;
  }

  Cursor end_finder = tryToReachAbsPosition(fc->cursor, INT_MAX, INT_MAX);
  int total_lines = cursor_row(end_finder);
  if (total_lines <= 0) {
    return false;
  }

  int start_row = cursor_row(fc->cursor);
  int start_col = cursor_col(fc->cursor);

  int current_row = start_row;
  bool first_line = true;
  int query_char_len = ilj_byteCountToCharCount(query);

  while (true) {
    Cursor temp_cursor = tryToReachAbsPosition(fc->cursor, current_row, 0);
    char* line_str = ilj_getLineString(temp_cursor.line_id.line);
    if (!line_str) {
      return false;
    }

    const char* best_match = NULL;
    const char* scan_ptr = line_str;

    while (true) {
      const char* match = NULL;
      if (case_sensitive) {
        match = strstr(scan_ptr, query);
      } else {
        match = ilj_strcasestr(scan_ptr, query);
      }
      if (!match) {
        break;
      }

      int byte_offset = match - line_str;
      int char_offset = ilj_byteOffsetToCharOffset(line_str, byte_offset);

      if (first_line && char_offset >= start_col) {
        break;
      }

      best_match = match;
      scan_ptr = match + 1;
    }

    if (best_match) {
      int byte_offset = best_match - line_str;
      int char_offset = ilj_byteOffsetToCharOffset(line_str, byte_offset);
      *start_cursor = tryToReachAbsPosition(fc->cursor, current_row, char_offset);
      *end_cursor = tryToReachAbsPosition(fc->cursor, current_row, char_offset + query_char_len);
      free(line_str);
      return true;
    }

    free(line_str);

    current_row--;
    first_line = false;

    if (current_row < 1) {
      if (wrap) {
        current_row = total_lines;
      } else {
        break;
      }
    }

    if (current_row == start_row && !first_line) {
      temp_cursor = tryToReachAbsPosition(fc->cursor, start_row, 0);
      line_str = ilj_getLineString(temp_cursor.line_id.line);
      if (line_str) {
        scan_ptr = line_str;
        best_match = NULL;
        while (true) {
          const char* match = NULL;
          if (case_sensitive) {
            match = strstr(scan_ptr, query);
          } else {
            match = ilj_strcasestr(scan_ptr, query);
          }
          if (!match) {
            break;
          }

          int byte_offset = match - line_str;
          int char_offset = ilj_byteOffsetToCharOffset(line_str, byte_offset);

          if (char_offset >= start_col) {
            best_match = match;
          }
          scan_ptr = match + 1;
        }

        if (best_match) {
          int byte_offset = best_match - line_str;
          int char_offset = ilj_byteOffsetToCharOffset(line_str, byte_offset);
          *start_cursor = tryToReachAbsPosition(fc->cursor, start_row, char_offset);
          *end_cursor = tryToReachAbsPosition(fc->cursor, start_row, char_offset + query_char_len);
          free(line_str);
          return true;
        }
        free(line_str);
      }
      break;
    }
  }

  return false;
}
