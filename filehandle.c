#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <ncurses.h>
#include "filehandle.h"

void error(char *message) {
	char error_string[256];
  sprintf(error_string, "%s: errno %d", message, errno);
  perror(error_string);
  exit(1);
}

FILE* myopen(char *filename) {
	// r+ is read and write
	FILE *file = fopen(filename, "r+");
	if (file == NULL) {
		error("Open file");
	}

	return file;
}

void myclose(FILE *file) {
	if (fclose(file) == EOF) {
		error("Close file");
	}
}

struct file_buffer create_file_buffer(int init_array_length) {
	struct file_buffer file_buff;
	file_buff.buffer = (char**) calloc(init_array_length, sizeof(char*));
	file_buff.array_length = init_array_length;
	file_buff.rows = 0;
	
	for (int r = 0; r < init_array_length; r++) {
		file_buff.buffer[r] = (char*) calloc(LINE_SIZE, sizeof(char));
	}
	
	return file_buff;
}

void read_into_buffer(FILE *file, struct file_buffer *file_buff) {
	if (fseek(file, 0, SEEK_SET) != 0) { error("read_into_buffer: fseek"); }

	char line[LINE_SIZE];
	for (file_buff->rows = 0; fgets(line, LINE_SIZE, file) != NULL; file_buff->rows++) {
		// if the last character is a '\n', strip it
		int length = strlen(line);
		if (line[length-1] == '\n') { line[length-1] = '\0'; }
		
		// grow the array if needed
		if (file_buff->rows >= file_buff->array_length) {
			file_buff->array_length = 2*file_buff->array_length + 1;
			file_buff->buffer = (char**) realloc(file_buff->buffer, file_buff->array_length);
		}

		line[LINE_SIZE-1] = '\0'; // safety null
		strncpy(file_buff->buffer[file_buff->rows], line, LINE_SIZE);
	}
}

void showall(struct file_buffer file_buff) {
	for (int r = 0; r < file_buff.rows; r++) {
		printf("'%s'\n", file_buff.buffer[r]);
	}
}

void insert_char(struct file_buffer file_buff, int r, int c, char ch) {
	if (r >= file_buff.rows) {
		printf("r shouldn't be greater than file_buff.rows, r=%d, file_buff.rows=%d\n", r, file_buff.rows);
		exit(1);
	}
	
	int length = strlen(file_buff.buffer[r]);
	printf("length: %d\n", length);
	if (c > length) {
		printf("c shouldn't be greater than the line length at r=%d, c=%d, length=%d\n", r, c, length);
		exit(1);
	}
	
	if (!(length+1 < LINE_SIZE)) {
		// maybe figure out how to realloc lines later
		printf("out of room");
		exit(1);
	}
	
	char temp;
	for (int i = c; i < length; i++) {
		temp = file_buff.buffer[r][i];
		file_buff.buffer[r][i] = ch;
		ch = temp;
	}
	// manual concatenation
	file_buff.buffer[r][length] = ch;
	file_buff.buffer[r][length+1] = '\0';
}

void insert_row(struct file_buffer file_buff, int r) {
	
}

void delete_char(struct file_buffer file_buff, int r, int c) {
	
}

void delete_row(struct file_buffer file_buff, int r) {
	
}