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
int read_into_buffer(FILE *file, char **buffer, int array_length) {
	// seek to start(SEEK_SET)
	if (fseek(file, 0, SEEK_SET) != 0) { error("read_into_buffer: fseek"); }

	// read line by line
	// fgets reads up until first newline, eof, or n bytes
	char line[LINE_SIZE];
		// char *line = calloc(LINE_SIZE, sizeof(char));

	for (int r = 0; fgets(line, LINE_SIZE, file) != NULL; r++) {
		printf("r: %d\n", r);
		// if the last character is a '\n' (usually is), strip it
		int length = strlen(line);
		if (line[length-1] == '\n') {
			line[length-1] = '\0';
		}

		printf("here 5\n");

		// sizeof on allocated memory is weird
		if (r >= array_length) {
			printf("???\n");
			array_length = array_length * 2 + 1; // arbitrary growth factor
			resize_2D_buffer(buffer, array_length);
		}

		printf("about to copy into buffer[r=%d]\n", r);
		printf("line: %s\n", line);
		line[LINE_SIZE-1] = '\0'; // safety null
		printf("nullified\n");
		for (int i = 0; i < LINE_SIZE; i++) {
			printf("i=%d: %c\n", i, line[i]);
			printf("r=%d\n", r);
			printf("%p\n", buffer[r]);
			buffer[r][i] = line[i];
			if (line[i] == NULL) { break; }
		}
		// strncpy(buffer[r], line, LINE_SIZE-1);
		printf("copied good\n");
		printf("buffer[r]: %s\n", buffer[r]);
	}

	printf("end of read_into_buffer\n");

	return array_length;
}

// rows may not be the size of buffer; there can be empty lines at the end
void showall(char **buffer, int rows) {
	for (int r = 0; r < rows; r++) {
		printf("'%s'\n", buffer[r]);
	}

}

char** init_2D_buffer(int rows, int cols) {
	char **buffer = (char**) calloc(rows, sizeof(char));
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
