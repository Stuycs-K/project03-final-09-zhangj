#ifndef WRITESAVE_H
#define WRITESAVE_H
#include <ncurses.h>
#include "filebuffer.h"
#include "utils.h"

void save(struct file_buffer *file_buff, char *filename);
void quit(struct file_buffer *file_buff, char* fname, int changed);

int check_can_save(WINDOW *win, struct file_buffer *file_buff, char **filename, int *show_message, char *message, int height, int width);

#endif
