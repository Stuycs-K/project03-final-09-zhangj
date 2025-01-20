#ifndef EXECPRINT_H
#define EXECPRINT_H

#include <unistd.h>
#include "filebuffer.h"

#define ARRAY_SIZE 16
#define READ_FD 0
#define WRITE_FD 1

void parse_args( char * line, char ** arg_ary );
void redirect_stdout(int fd);
void do_stuff(struct file_buffer *file_buff, int *pipe_fds, char **arg_array, char *error_message, int *x, int *y, int *height, int *width, int *curY, int *yLineEnd, int *top, int *bottom, int *has_error, int *changed);

#endif
