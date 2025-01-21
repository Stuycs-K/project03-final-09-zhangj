#ifndef CURSOR_H
#define CURSOR_H

#include "filebuffer.h"
#include <ncurses.h>

int keyleft(int x);
int keyright(int x, int xLineEnd);
int keyup(int* x, int y, int lineLen, int* curY);
int keydown(int* x, int y, int yLineEnd, int numrows, int linelen, int* curY);

void move_cursor(int c, int *x, int *y, int *curY, int xLineEnd, int yLineEnd, struct file_buffer *file_buff);

void do_goto(WINDOW *win, int height, int width, int *x, int *y, int *show_message, struct file_buffer *file_buff, char *message);

#endif
