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
#include "filebuffer.h"

// creates a struct file_buffer, initializing its contents:
// buffer: init_array_length amount of LINE_SIZE length rows
// array_length: init_array_length
// rows: 0
struct file_buffer* create_file_buffer(int init_array_length) {
	struct file_buffer *file_buff = (struct file_buffer*) malloc(1 * sizeof(struct file_buff*));
	file_buff->buffer = (char**) malloc(init_array_length * sizeof(char*));
	file_buff->array_length = init_array_length;
	file_buff->rows = 0;

	for (int r = 0; r < init_array_length; r++) {
		file_buff->buffer[r] = (char*) malloc(LINE_SIZE * sizeof(char));
	}

	return file_buff;
}

void resize(struct file_buffer *file_buff) {
	file_buff->array_length = 2*file_buff->array_length + 1;
	char **new_buffer = (char**) realloc(file_buff->buffer, file_buff->array_length * sizeof(char*));
	if (new_buffer == NULL) {
		endwin();
		fprintf(stderr, "resize: realloc failed\nerrno %d: %s\n", errno, strerror(errno));
		exit(1);
	} else {
		file_buff->buffer = new_buffer;
	}

	for (int r = file_buff->rows; r < file_buff->array_length; r++) {
		file_buff->buffer[r] = (char*) malloc(LINE_SIZE * sizeof(char));
	}
}

// note: all these insert / delete functions use a O(n) shift for EVERY character which is really bad even for relatively short strings, but maybe it's fine

// inserts a character ch such that it is at buffer[r][c] after insertion
void insert_char(struct file_buffer *file_buff, int r, int c, char ch) {
	if (!(0 <= r && r < file_buff->rows)) {
		endwin();
		fprintf(stderr, "r=%d is out of bounds, expected value between 0 and rows=%d\n", r, file_buff->rows);
		exit(1);
	}

	int length = strlen(file_buff->buffer[r]);
	if (!(c <= length)) {
		endwin();
		fprintf(stderr, "c=%d is out of bounds, expected value between 0 and length=%d\n", c, length);
		exit(1);
	}

	if (!(length+1 < LINE_SIZE)) {
		// maybe figure out how to realloc lines later
		endwin();
		printf("out of room");
		return;
	}

	char temp;
	for (int i = c; i < length; i++) {
		temp = file_buff->buffer[r][i];
		file_buff->buffer[r][i] = ch;
		ch = temp;
	}
	// manual concatenation
	file_buff->buffer[r][length] = ch;
	file_buff->buffer[r][length+1] = '\0';
}

// inserts an empty row such that it is at buffer[r] after insertion
void insert_row(struct file_buffer *file_buff, int r) {
	if (!(0 <= r && r <= file_buff->rows)) {
		endwin();
		fprintf(stderr, "r=%d is out of bounds, expected value between 0 and rows=%d\n", r, file_buff->rows);
		exit(1);
	}

	file_buff->rows++;
	if (file_buff->rows == file_buff->array_length) {
		resize(file_buff);
	}

	char *temp;
	char *line = (char*) malloc(LINE_SIZE * sizeof(char));
	for (int i = r; i < file_buff->rows; i++) {
		temp = file_buff->buffer[i];
		file_buff->buffer[i] = line;
		line = temp;
	}

	file_buff->buffer[file_buff->rows] = line;
}

// removes the character at buffer[r][c], left-shifting over all the characters to its right
void delete_char(struct file_buffer *file_buff, int r, int c) {
	if (!(0 <= r && r < file_buff->rows)) {
		endwin();
		fprintf(stderr, "r=%d is out of bounds, expected value between 0 and rows=%d\n", r, file_buff->rows);
		exit(1);
	}

	int length = strlen(file_buff->buffer[r]);
	if (!(c < length)) {
		endwin();
		fprintf(stderr, "c=%d is out of bounds, expected value between 0 and length=%d\n", c, length);
		exit(1);
	}

	for (int i = c; i < length-1; i++) {
		file_buff->buffer[r][i] = file_buff->buffer[r][i+1];
	}
	file_buff->buffer[r][length-1] = '\0';
}

// removes the row at buffer[r], shifting all the rows after it
void delete_row(struct file_buffer *file_buff, int r) {
	if (!(0 <= r && r < file_buff->rows)) {
		endwin();
		fprintf(stderr, "r=%d is out of bounds, expected value between 0 and rows=%d\n", r, file_buff->rows);
		exit(1);
	}

	file_buff->rows--;
	for (int i = r; i < file_buff->rows; i++) {
		file_buff->buffer[i] = file_buff->buffer[i+1];
	}

	for (int i = 0; i < LINE_SIZE; i++) {
		file_buff->buffer[file_buff->rows+1][i] = '\0';
	}
}

// need special case for newline char
void insert_newline(struct file_buffer *file_buff, int r, int c) {
	if (!(0 <= r && r < file_buff->rows)) {
		endwin();
		fprintf(stderr, "r=%d is out of bounds, expected value between 0 and rows=%d\n", r, file_buff->rows);
		exit(1);
	}

	int length = strlen(file_buff->buffer[r]);
	if (!(c <= length)) {
		endwin();
		fprintf(stderr, "c=%d is out of bounds, expected value between 0 and length=%d\n", c, length);
		exit(1);
	}

	char *new_line = (char*) malloc(LINE_SIZE * sizeof(char));

	int j;
	for (j = 0; j+c < length; j++) {
		new_line[j] = file_buff->buffer[r][j+c];
	}
	new_line[j] = '\0';

	file_buff->buffer[r][c] = '\n';
	file_buff->buffer[r][c+1] = '\0';

	(file_buff->rows)++;
	if (file_buff->rows == file_buff->array_length) {
		resize(file_buff);
	}

	char *temp;
	for (int i = r+1; i < file_buff->rows-1; i++) {
		temp = file_buff->buffer[i];
		file_buff->buffer[i] = new_line;
		new_line = temp;
	}

	file_buff->buffer[file_buff->rows-1] = new_line;
}

void delete_newline(struct file_buffer *file_buff, int r) {
	if (!(0 < r && r < file_buff->rows)) {
		endwin();
		fprintf(stderr, "r=%d is out of bounds, expected value 0 < r < rows=%d\n", r, file_buff->rows);
		exit(1);
	}

	int length = strlen(file_buff->buffer[r-1]) - 1; // -1 to overwrite newline
	//printf("length is :%d\n", length);
	int i;
	for (i = 0; i+length < LINE_SIZE && file_buff->buffer[r][i] != '\0'; i++) {
		//printf("i=%d: ascii %d\n", i, file_buff->buffer[r-1][i+length]);
		file_buff->buffer[r-1][i+length] = file_buff->buffer[r][i];
		//printf("i=%d: ascii %d\n", i, file_buff->buffer[r-1][i+length]);
	}

	
	//printf("length+i+1: %d\n", file_buff->buffer[r-1][length+i+1]);
	//printf("length+i-1: %d\n", file_buff->buffer[r-1][length+i-1]);
	file_buff->buffer[r-1][length+i] = '\0';

	free(file_buff->buffer[r]);

	file_buff->rows--;
	for (i = r; i < file_buff->rows; i++) {
		file_buff->buffer[i] = file_buff->buffer[i+1];
		printf("i=%d: %s\n", i, file_buff->buffer[i]);
	}

	file_buff->buffer[file_buff->rows] = (char*) malloc(LINE_SIZE * sizeof(char));
}

// takes a file pointer and a struct file_buffer, and reads in the full contents of the file into the file_buffer's char** buffer
void read_into_buffer(FILE *file, struct file_buffer *file_buff, int winLen) {
	if (fseek(file, 0, SEEK_SET) != 0) {
		endwin();
		fprintf(stderr, "read_into_buffer: failed to fseek to 0, SEEK_SET\nerrno %d: %s\n", errno, strerror(errno));
		exit(1);
	}

	char line[LINE_SIZE];
	int length;
	while(fgets(line, LINE_SIZE, file) != NULL) {
		// grow the array if needed
		if (file_buff->rows >= file_buff->array_length) {
			resize(file_buff);
		}

		line[LINE_SIZE-1] = '\0'; // safety null
		strncpy(file_buff->buffer[file_buff->rows], line, LINE_SIZE);
		file_buff->rows++;

		length = strlen(file_buff->buffer[file_buff->rows-1]);
		while (length >= winLen-7){
			if (file_buff->rows >= file_buff->array_length) {
				resize(file_buff);
			}
			insert_newline(file_buff,file_buff->rows-1,winLen-9);
			file_buff->rows++;
			insert_char(file_buff,file_buff->rows-2,strlen(file_buff->buffer[file_buff->rows-2]),'-');
			insert_char(file_buff,file_buff->rows-2,strlen(file_buff->buffer[file_buff->rows-2]),'\n');
			length = strlen(file_buff->buffer[file_buff->rows-2]);
		}
	}
}