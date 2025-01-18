#ifndef WRITESAVE_H
#define WRITESAVE_H
#include "filebuffer.h"

void save(struct file_buffer *file_buff, char *filename);
void quit(struct file_buffer *file_buff, char* fname, int changed);

#endif
