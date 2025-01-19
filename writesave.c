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
#include "writesave.h"

// Takes in the buffer and writes it into a file,
void save(struct file_buffer *file_buff, char *filename) {
  // w+ truncatres
  FILE *file = fopen(filename, "w+");

  char newline[] = "\n";

  for (int r = 0; r < file_buff->rows; r++) {
    int length = strlen(file_buff->buffer[r]);
    while (file_buff->buffer[r][length-2] == '-' && file_buff->buffer[r][length-1] == '\n'){
      delete_char(file_buff,r,length-1);
      delete_char(file_buff,r,length-1);
      delete_newline(file_buff,r+1);
      length = strlen(file_buff->buffer[r]);
    }
    int elements = fwrite(file_buff->buffer[r], sizeof(char), length, file);
    if (elements != length) {
      printf("fwrite wrote incorrect number of elements=%d, expected %d\n", elements, length);
      exit(1);
    }

    if (r == file_buff->rows-1){
      elements = fwrite(newline, sizeof(char), 1, file);
    }
  }

  close_file(file);
}

// Quits out of raw mode and prompts user if they want to save the changes and asks them if they want to rename the file before quitting
void quit(struct file_buffer *file_buff, char* fname, int changed) {
  endwin();
  if (changed == 1){
    printf("Would you like to save your modified changes? y/n ");
    char response[256];
    response[256] = '\0';
    fgets(response, 255, stdin);
    if (response[0] == 'y'){
      if (strcmp(fname,"Untitled.txt") == 0){
        printf("File name: ");
        char newfname[256];
        fgets(newfname, 255, stdin);
        char* newfname1 = (char *) malloc(256);
        sscanf(newfname, "%[^\n]", newfname1);
        if (strlen(newfname1) <= 0){
          remove("Untitled.txt");
          printf("Filename must be greater than length 0\n");
          exit(1);
        }
        if (strcmp(newfname1,"Untitled.txt")==0){
          remove("Untitled.txt");
          printf("Filename cannot be Untitled.txt\n");
          exit(1);
        }
        save(file_buff, newfname1);
        free(newfname1);
        remove("Untitled.txt");
      }
      else{
        save(file_buff, fname);
      }
    }
    else if (response[0] == 'n'){
        if (strcmp(fname,"Untitled.txt") == 0){
          remove("Untitled.txt");
        }
        printf("Quitting without saving...\n");
    }
    else{
        printf("Invalid input. Quitting without saving...\n");
    }
  }
  else{
    if (strcmp(fname,"Untitled.txt")==0){
      remove("Untitled.txt");
    }
    printf("Quitting... (no changes were made since last save)\n");
  }
}
