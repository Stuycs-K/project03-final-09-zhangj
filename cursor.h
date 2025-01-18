#ifndef CURSOR_H
#define CURSOR_H

int keyleft(int x, int y);
int keyright(int x, int xLineEnd);
int keyup(int* x, int y, int lineLen, int* curY);
int keydown(int* x, int y, int yLineEnd, int numrows, int linelen, int* curY);

#endif
