#ifndef WRITESAVE_H
#define WRITESAVE_H
#include "filehandle.h"

void quit(struct file_buffer *file_buff);
void save(char *filename, struct file_buffer *file_buff);

#endif
