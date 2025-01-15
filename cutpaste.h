#ifndef CUTPASTE_H
#define CUTPASTE_H
#include "filehandle.h"

void cut_string(char *copy_buffer, struct file_buffer *file_buff, int r1, int c1, int r2, int c2, int remove_string);
void paste_string(char *copy_buffer, struct file_buffer *file_buff, int r, int c);

#endif
