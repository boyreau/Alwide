#ifndef THEME_H
#define THEME_H
#include <ncurses.h>
#include <stdbool.h>

typedef struct {
  short r, g, b;
} Color;

typedef struct {
  Color color;
  int color_n;
  Color color_hover;
  int color_hover_n;
  char attr[10];
  char group[256];
} HighlightTheme;

typedef struct {
  HighlightTheme* groups;
  int size;
} HighlightThemeList;

bool getThemeFromFile(char* file_name, HighlightThemeList* list);

void initHighlightThemeList(HighlightThemeList* list);

void destroyThemeList(HighlightThemeList* list);

void addToHiglightThemeList(HighlightThemeList* list, HighlightTheme theme);

attr_t getAttrForTheme(HighlightTheme theme);


#endif // THEME_H
