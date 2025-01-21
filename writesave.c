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
#include "filehandle.h"
#include "writesave.h"

// Takes in the file buffer and writes it into the file specified by file name
void save(struct file_buffer *file_buff, char *filename) {
  // w+ truncates file
  FILE *file = fopen(filename, "w+");

  for (int r = 0; r < file_buff->rows; r++) {
    int length = strlen(file_buff->buffer[r]);
    if (file_buff->buffer[r][length-2] == '-' && file_buff->buffer[r][length-1] == '\n'){ // Concatenate long lines into one line if they exist
      delete_char(file_buff,r,length-2);
      delete_newline(file_buff,r+1);
      length = strlen(file_buff->buffer[r]);
    }
    int elements = fwrite(file_buff->buffer[r], sizeof(char), length, file); // Write line of buffer to file
    if (elements != length) {
      printf("fwrite wrote incorrect number of elements=%d, expected %d\n", elements, length);
      exit(1);
    }
  }

  close_file(file);
}

// Quits out of raw mode and prompts user if they want to save the changes and asks them if they want to rename the file 
// (and/or overwrite a file if applicable) before quitting
void quit(struct file_buffer *file_buff, char* fname, int changed) {
  endwin(); // Exit raw mode
  struct stat *stat_buffer = malloc(sizeof(struct stat));
  stat(fname, stat_buffer);
  int bytes = stat_buffer->st_size;
  if (changed == 1){ // If changes were made to the file
    printf("Would you like to save your modified changes? y/n "); // Prompt user to save or not
    char response[LINE_SIZE];
    fgets(response, LINE_SIZE, stdin);
    response[LINE_SIZE-1] = '\0';
    if (response[0] == 'y'){ // If user desire to save
      if (strcmp(fname,UNTITLED_FILENAME) == 0){ // Prompt user for file name if there was no file name specified previously
        printf("File name: "); 
        char newfname[LINE_SIZE];
        fgets(newfname, LINE_SIZE, stdin);
        char* newfname1 = (char *) malloc(LINE_SIZE * sizeof(char));
        sscanf(newfname, "%[^\n]", newfname1);
				int can_save = 1;
        if (strlen(newfname1) <= 0){
          remove(UNTITLED_FILENAME);
					fprintf(stderr, "Error: 0-length filename. Not saving.\n");
          exit(1);
        }
				else if (strcmp(newfname1,UNTITLED_FILENAME)==0){
          remove(UNTITLED_FILENAME);
					fprintf(stderr, "Filename cannot be %s. Not saving. \n", UNTITLED_FILENAME);
          exit(1);
        }
				else if (access(newfname1, F_OK) != -1){  // check if the file exists using access()
					char *secondline = (char*) malloc(LINE_SIZE * sizeof(char));
					printf("The file \"%s\" already exists. Overwrite it? (y/n): ", newfname1); // Ask user if they want to overwrite the file if it does already exist
					fgets(secondline, LINE_SIZE, stdin);
					if (secondline[0] == '\0') {
						fprintf(stderr, "Error: 0-length filename. Not saving.\n");
						exit(1);
					} else if (secondline[0] == 'y') { // Save file if yes
						can_save = 1;
					} else if (secondline[0] == 'n') { // Don't save file if no
						can_save = 0;
						free(secondline);
						free(newfname1);
						remove(UNTITLED_FILENAME);
            printf("Quitting without saving...\n");
					} else { // Catch invalid input
						fprintf(stderr, "Error: Invalid input '%s'. Not saving.\n", secondline);
						exit(1);
					}
				}		


				if (can_save) { // Save file to file name specified from earlier user input
	        save(file_buff, newfname1);
          printf("Saving file to %s...\n", newfname1);
	        free(newfname1);
	        remove(UNTITLED_FILENAME);
				}
      }
      else{ // Save file to file name specified from make argument
        save(file_buff, fname);
        printf("Saving file to %s...\n", fname);
      }
    }
    else if (response[0] == 'n'){ // Don't save file if no
      if (strcmp(fname,UNTITLED_FILENAME) == 0){
        remove(UNTITLED_FILENAME);
      }
      else if (bytes == 0){
        remove(fname);
      }
      printf("Quitting without saving...\n");
    }
    else{ // Catch invalid input
      if (strcmp(fname,UNTITLED_FILENAME) == 0){
        remove(UNTITLED_FILENAME);
      }
      else if (bytes == 0){
        remove(fname);
      }
      fprintf(stderr, "Error: Invalid input '%s'. Not saving.\n", response);
    }
  }
  else{ // Quit without prompting user to save changes if no changes were made
    if (strcmp(fname,UNTITLED_FILENAME)==0){
      remove(UNTITLED_FILENAME);
    }
    else if (bytes == 0){
      remove(fname);
    }
    printf("Quitting... (no changes were made since last save)\n");
  }
}

int check_can_save(WINDOW *win, struct file_buffer *file_buff, char **filename, int *show_message, char *message, int height, int width) {
	int can_save = 1;
	if (strcmp(*filename, UNTITLED_FILENAME) == 0){
		char *line = malloc(256 * sizeof(char));
		clear_fgets_line(win, height, width);
		mvwprintw(win, height-2, 0, "(Save) Enter Filename: ");
		wrefresh(win);
		
		my_fgets(win, line, height, 32, 126, 23);
		// check that:
		// - the filename is not empty and
		// - the filename is not "untitled.txt"
		if (strcmp(line, "") == 0) {
			*show_message = 1;
			sprintf(message, "Error: cannot use empty string as filename.");
			can_save = 0;
		}
		else if (strcmp(line, UNTITLED_FILENAME) == 0) {
			*show_message = 1;
			sprintf(message, "Error: cannot use %s as filename.", UNTITLED_FILENAME);
			can_save = 0;
		} else if (access(line, F_OK) != -1){  // check if the file exists using access()
			char *secondline = (char*) malloc(LINE_SIZE * sizeof(char));
			clear_fgets_line(win, height, width);
			mvwprintw(win, height-2, 0, "The file \"%s\" already exists. Overwrite it? (y/n): ", line);
			wrefresh(win);
			my_fgets(win, secondline, height, 32, 126, 23);
			if (secondline[0] == '\0') {
				sprintf(message, "Error: no response to y/n.");
				*show_message = 1;
				can_save = 0;
			} else if (secondline[0] == 'y') {
				can_save = 1;
				free(filename);
				*filename = line;
				remove(UNTITLED_FILENAME);
			} else if (secondline[0] == 'n') {
				can_save = 0;
			} else {
				sprintf(message, "Error: did not recognize response '%s'. Not saving.", secondline);
				*show_message = 1;
				can_save = 0;
			}
		} else {
			free(*filename);
			*filename = line;
		}		
	}

	return can_save;
}