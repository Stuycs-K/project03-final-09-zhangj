#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>

// these are random functions used in main.c

int to_ctrl_char(int ch);
void clear_fgets_line(WINDOW *win, int height, int width);
void my_fgets(WINDOW *win, char *line, int height, int char_range_min, int char_range_max, int promptLen);

#endif