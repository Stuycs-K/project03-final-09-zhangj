#ifndef CURSOR_H
#define CURSOR_H

int keyleft(int x);
int keyright(int x, int xLineEnd);
int keyup(int* x, int y, struct file_buffer *file_buff);

#endif