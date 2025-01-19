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
#include "statdisplay.h"

// returns a file pointer to the file, opened only in read mode
FILE* open_read(char *filename) {
	FILE *file;

	// create the file if it does not exist
	file = fopen(filename, "a");
	if (file == NULL) {
		endwin();
		fprintf(stderr, "open_read: failed to create file\nerrno %d: %s\n", errno, strerror(errno));
		exit(1);
	}
	close_file(file);

	file = fopen(filename, "r");
	if (file == NULL) {
		endwin();
		fprintf(stderr, "open_read: failed to open file\nerrno %d: %s\n", errno, strerror(errno));
		exit(1);
	}

	return file;
}

// closes a file pointer
void close_file(FILE *file) {
	if (fclose(file) == EOF) {
		endwin();
		fprintf(stderr, "close_file: failed to close file\nerrno %d: %s\n", errno, strerror(errno));
		exit(1);
	}
}

// Initialize file
FILE* initFile(int argc, char *argv[], char* filename, char* fileinfo){
  FILE *file;
  if (argc == 1){
		sprintf(filename, "Untitled.txt");
		file = fopen(filename, "w+");
		close_file(file);
		stat_info(filename, fileinfo);
	}
	else if (argc == 2){
		sprintf(filename,"%s",argv[1]);
		file = open_read(filename);
		*fileinfo = stat_info(argv[1], fileinfo);
	}
	else {
		printf("Incorrect number of arguments");
		exit(1);
	}
  return file;
}