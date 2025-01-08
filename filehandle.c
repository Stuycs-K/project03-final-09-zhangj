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
	FILE *file = fopen("test.txt", "w");
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

void showall(int file_descriptor) {


}
