#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>
// these are random functions and #defines used in main.c

// to display the cursor position and controls
#define TOP_OFFSET 1
// to display the file information and asking for prompts / showing error messages
#define BOTTOM_OFFSET 2
#define TOTAL_OFFSET (TOP_OFFSET + BOTTOM_OFFSET)

#define TOP_DISPLAY_MESSAGE "%d:%d| Ctrl+Q - Quit  Ctrl+S - Save  Ctrl+T - Execute  Ctrl+G - Go to line #\n"

// the length of the line number string e.g. "001| "
#define OFFSET_INIT 5 

int to_ctrl_char(int ch);
void clear_fgets_line(WINDOW *win, int height, int width);
void my_fgets(WINDOW *win, char *line, int height, int char_range_min, int char_range_max, int promptLen);

#endif