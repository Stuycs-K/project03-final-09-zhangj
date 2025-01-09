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

// buffer will be a dynamically sized array of char[]s that each represent a line
struct something read_into_buffer(FILE *file, char **buffer, int array_length) {
	if (fseek(file, 0, SEEK_SET) != 0) { error("read_into_buffer: fseek"); }

	int rows;
	// read line by line
	char line[LINE_SIZE];
	for (;fgets(line, LINE_SIZE, file) != NULL; rows++) {
		// if the last character is a '\n' (usually is), strip it
		int length = strlen(line);
		if (line[length-1] == '\n') {
			line[length-1] = '\0';
		}

		// sizeof on allocated memory is weird
		if (rows >= array_length) {
			array_length = array_length * 2 + 1; // arbitrary growth factor
			resize_2D_buffer(buffer, array_length);
		}

		line[LINE_SIZE-1] = '\0'; // safety null
		strncpy(buffer[rows], line, LINE_SIZE);
	}

	struct something smth = {array_length, rows};
	return smth;
}

// rows may not be the size of buffer; there can be empty lines at the end
void showall(char **buffer, int rows) {
	for (int r = 0; r < rows; r++) {
		printf("'%s'\n", buffer[r]);
	}

}

char** init_2D_buffer(int rows, int cols) {
	char **buffer = (char**) calloc(rows, sizeof(char*));
	for (int r = 0; r < rows; r++) {
		buffer[r] = (char*) calloc(cols, sizeof(char));
		printf("buffer[%d] allocated successfully with cols=%d\n", r, cols);
	}

	return buffer;
}

char** resize_2D_buffer(char **buffer, int rows) {
	return (char**) realloc(buffer, rows);
}

char* resize_1D_buffer(char *buffer, int cols) {
	return buffer;
}
