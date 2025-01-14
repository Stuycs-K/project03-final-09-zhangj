#ifndef WRITESAVE_H
#define WRITESAVE_H
#include "filehandle.h"

void save(char *filename, struct file_buffer *file_buff);
void quit(struct file_buffer *file_buff, char* fname);

#endif
