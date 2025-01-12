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

// Quits out of raw mode and prompts user if they want to save the changes and asks them if they want to rename the file before quitting
void quit(struct file_buffer *file_buff) {
  endwin();
  printf("Would you like to save your modified changes? y/n ");
  char buff[256];
  buff[256] = '\0';
  fgets(buff, 255, stdin);
  if (buff[0] == 'y'){
      printf("File name: ");
      char buff1[256];
      fgets(buff1, 255, stdin);
      save(buff1, *file_buff);
  }
  else if (buff[0] == 'n'){
      printf("Quitting...\n");
  }
  else{
      printf("Invalid input. Quitting without saving...\n");
  }
}

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
