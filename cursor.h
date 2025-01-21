#ifndef CURSOR_H
#define CURSOR_H

#include "filebuffer.h"

int keyleft(int x);
int keyright(int x, int xLineEnd);
int keyup(int* x, int y, int lineLen, int* curY);
int keydown(int* x, int y, int yLineEnd, int numrows, int linelen, int* curY);

void move_cursor(int c, int *x, int *y, int *curY, int xLineEnd, int yLineEnd, struct file_buffer *file_buff);

#endif
