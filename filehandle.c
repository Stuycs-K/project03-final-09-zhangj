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
void read_into_buffer(FILE *file, char **buffer) {
	// seek to start(SEEK_SET)
	if (fseek(file, 0, SEEK_SET) != 0) { error("read_into_buffer: fseek"); }
	// if not enough space, realloc
	
	// read line by line
	// fgets reads up until first newline, eof, or n bytes
	char *line = calloc(LINE_SIZE, sizeof(char));
	
	for (int r = 0; fgets(line, LINE_SIZE, file) != NULL; r++) {
		// if the last character is a '\n' (usually is), strip it, it will be included in the showall function
		int length = strlen(line);
		if (line[length-1] == '\n') {
			line[length-1] = '\0';
		}
		strcpy(buffer[r], line);
	}
	
	printf("end of read_into_buffer\n");
}

void showall(char **buffer, int rows) {
	for (int r = 0; r < rows; r++) {
		printf("'%s'\n", buffer[r]);
	}

}

char** init_2D_buffer(int rows, int cols) {
	char **buffer = (char**) calloc(rows, sizeof(char));
	for (int r = 0; r < rows; r++) {
		buffer[r] = (char*) calloc(cols, sizeof(char));
	}
	
	return buffer;
}

char** resize_2D_buffer(char **buffer, int rows) {
	return buffer;
}

char* resize_1D_buffer(char *buffer, int cols) {
	return buffer;
}