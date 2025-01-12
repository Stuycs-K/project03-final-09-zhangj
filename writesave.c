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

// Takes in the buffer and writes it into a file,
void save(char *filename, struct file_buffer *file_buff) {
  // w+ truncatres
  FILE *file = fopen(filename, "w+");

  char newline[] = "\n";

  //size_t written = fwrite(arr, sizeof(int), n, fp);
  for (int r = 0; r < file_buff->rows; r++) {
    int length = strlen(file_buff->buffer[r]);
    int elements = fwrite(file_buff->buffer[r], sizeof(char), length, file);
    if (elements != length) {
      printf("fwrite wrote incorrect number of elements=%d, expected %d\n", elements, length);
      exit(1);
    }


    elements = fwrite(newline, sizeof(char), 1, file);
    if (elements != 1) {
      printf("fwrite wrote incorrect number of elements=%d, expected %d\n", elements, 1);
      exit(1);
    }
  }

  close_file(file);
}

// Quits out of raw mode and prompts user if they want to save the changes and asks them if they want to rename the file before quitting
void quit(struct file_buffer *file_buff) {
  endwin();
  printf("Would you like to save your modified changes? y/n ");
  char response[256];
  response[256] = '\0';
  fgets(response, 255, stdin);
  if (response[0] == 'y'){
      printf("File name: ");
      char fname[256];
      fgets(fname, 255, stdin);
      char* fname1 = strdup(fname);
      save(fname1, file_buff);
  }
  else if (response[0] == 'n'){
      printf("Quitting...\n");
  }
  else{
      printf("Invalid input. Quitting without saving...\n");
  }
}
