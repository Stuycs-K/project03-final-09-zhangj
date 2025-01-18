#ifndef FILEHANDLE_H
#define FILEHANDLE_H
#include <stdio.h>

#define LINE_SIZE 1000

FILE* open_read(char *filename);
void close_file(FILE *file);

// array_length is the length of the array;
// rows is how many non-empty lines are in the buffer
struct file_buffer {
	char **buffer;
	int array_length;
	int rows;
};

struct file_buffer* create_file_buffer(int init_array_length);
void read_into_buffer(FILE *file, struct file_buffer *file_buff);

void insert_char(struct file_buffer *file_buff, int r, int c, char ch);
void insert_row(struct file_buffer *file_buff, int r);
void delete_char(struct file_buffer *file_buff, int r, int c);
void delete_row(struct file_buffer *file_buff, int r);

void insert_newline(struct file_buffer *file_buff, int r, int c);
void delete_newline(struct file_buffer *file_buff, int r);

#endif
