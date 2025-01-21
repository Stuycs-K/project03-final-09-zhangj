#ifndef EXECPRINT_H
#define EXECPRINT_H

#include <unistd.h>
#include <sys/wait.h>
#include <ncurses.h>
#include "filebuffer.h"

#define ARRAY_SIZE 16
#define READ_FD 0
#define WRITE_FD 1

void parse_args( char * line, char ** arg_ary );
void redirect_stdout(int fd);
void do_exec(struct file_buffer *file_buff, char **arg_array, char *error_message, int *x, int *y, int *height, int *width, int *curY, int *yLineEnd, int *top, int *bottom, int *has_error, int *changed);
void prep_exec(WINDOW *win, int height, int width, char *command_line, char **arg_array);

#endif
