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

// Takes in the buffer and writes it into a file,
void save(struct file_buffer *file_buff, char *filename) {
  // w+ truncates
  FILE *file = fopen(filename, "w+");

  for (int r = 0; r < file_buff->rows; r++) {
    int length = strlen(file_buff->buffer[r]);
    while (file_buff->buffer[r][length-2] == '-' && file_buff->buffer[r][length-1] == '\n'){
      delete_char(file_buff,r,length-2);
      delete_newline(file_buff,r+1);
      length = strlen(file_buff->buffer[r]);
    }
    int elements = fwrite(file_buff->buffer[r], sizeof(char), length, file);
    if (elements != length) {
      printf("fwrite wrote incorrect number of elements=%d, expected %d\n", elements, length);
      exit(1);
    }
  }

  close_file(file);
}

// Quits out of raw mode and prompts user if they want to save the changes and asks them if they want to rename the file before quitting
void quit(struct file_buffer *file_buff, char* fname, int changed) {
  endwin();
  if (changed == 1){
    printf("Would you like to save your modified changes? y/n ");
    char response[LINE_SIZE];
    fgets(response, LINE_SIZE, stdin);
    response[LINE_SIZE-1] = '\0';
    if (response[0] == 'y'){
      if (strcmp(fname,UNTITLED_FILENAME) == 0){
        printf("File name: ");
        char newfname[LINE_SIZE];
        fgets(newfname, LINE_SIZE, stdin);
        char* newfname1 = (char *) malloc(LINE_SIZE * sizeof(char));
        sscanf(newfname, "%[^\n]", newfname1);
        if (strlen(newfname1) <= 0){
          remove(UNTITLED_FILENAME);
          printf("Filename must be greater than length 0\n");
          exit(1);
        }
        if (strcmp(newfname1,UNTITLED_FILENAME)==0){
          remove(UNTITLED_FILENAME);
          fprintf(stderr, "Filename cannot be %s\n", UNTITLED_FILENAME);
          exit(1);
        }
        save(file_buff, newfname1);
        free(newfname1);
        remove(UNTITLED_FILENAME);
      }
      else{
        save(file_buff, fname);
      }
    }
    else if (response[0] == 'n'){
        if (strcmp(fname,UNTITLED_FILENAME) == 0){
          remove(UNTITLED_FILENAME);
        }
        printf("Quitting without saving...\n");
    }
    else{
        printf("Invalid input. Quitting without saving...\n");
    }
  }
  else{
    if (strcmp(fname,UNTITLED_FILENAME)==0){
      remove(UNTITLED_FILENAME);
    }
    printf("Quitting... (no changes were made since last save)\n");
  }
}
