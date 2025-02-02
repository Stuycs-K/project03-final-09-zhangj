#ifndef FILEBUFFER_H
#define FILEBUFFER_H
#include <stdio.h>

#define LINE_SIZE 1000

// array_length is the length of the array;
// rows is how many non-empty lines are in the buffer
struct file_buffer {
	char **buffer;
	int array_length;
	int rows;
};

struct file_buffer* create_file_buffer(int init_array_length);
void read_into_buffer(FILE *file, struct file_buffer *file_buff, int winLen);

void insert_char(struct file_buffer *file_buff, int r, int c, char ch);
void insert_row(struct file_buffer *file_buff, int r);
void delete_char(struct file_buffer *file_buff, int r, int c);
void delete_row(struct file_buffer *file_buff, int r);

void insert_newline(struct file_buffer *file_buff, int r, int c);
void delete_newline(struct file_buffer *file_buff, int r);

void insert_at_end(struct file_buffer *file_buff, char *line);

void do_delete(struct file_buffer *file_buff, int *x, int *y, int *curY, int *yLineEnd, int *longLine);
void do_insert_special(int c, struct file_buffer *file_buff, int *x, int *y, int *curY, int *xLineEnd, int *yLineEnd, int *longLine, int *show_message, char *message, int offset, int width);
void do_insert_regular(int c, struct file_buffer *file_buff, int *x, int *y, int *curY, int *xLineEnd, int *yLineEnd, int *longLine, int *show_message, char *message, int offset, int width);

#endif
