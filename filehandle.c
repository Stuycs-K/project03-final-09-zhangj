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

// returns a file pointer to the file with file name, opened in only read mode
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

	// Open the file in read mode
	file = fopen(filename, "r");
	if (file == NULL) {
		endwin();
		fprintf(stderr, "open_read: failed to open file\nerrno %d: %s\n", errno, strerror(errno));
		exit(1);
	}

	return file;
}

// Closes a file pointer
void close_file(FILE *file) {
	if (fclose(file) == EOF) {
		endwin();
		fprintf(stderr, "close_file: failed to close file\nerrno %d: %s\n", errno, strerror(errno));
		exit(1);
	}
}

// Initializes file from the file name specified by the make argument if it exists, sets the file name for file info to stat
FILE* init_file(int argc, char **argv, char* filename, char* fileinfo){
  FILE *file;
  if (argc == 1){ // If make is run with no argument
		sprintf(filename, UNTITLED_FILENAME); // File name is "Untitled.txt"
		file = fopen(filename, "w+");
		close_file(file);
		stat_info(filename, fileinfo);
	}
	else if (argc == 2){ // If make is run with a file name as its only argument
		if (strcmp(argv[1], UNTITLED_FILENAME) == 0) {
			fprintf(stderr, "Error: cannot open file as '%s'\n", UNTITLED_FILENAME);
			exit(1);
		}
		sprintf(filename,"%s",argv[1]);
		file = open_read(filename);
		stat_info(filename, fileinfo);
	}
	else { // If make is run with more than one argument
		fprintf(stderr, "Incorrect number of arguments. The program either accepts one argument as the file name or no arguments.\n");
		exit(1);
	}
  return file;
}
