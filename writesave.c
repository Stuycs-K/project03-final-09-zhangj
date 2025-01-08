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

// Quits out of raw mode and prompts user if they want to save the changes and asks them if they want to rename the file before quitting
void quit() {
  endwin();
  printf("Would you like to save your modified changes? y/n ");
  char buff[256];
  buff[256] = '\0';
  fgets(buff, 255, stdin);
  if (buff[0] == 'y'){
      printf("File name: ");
      char buff1[256];
      fgets(buff1, 255, stdin);
  }
  else if (buff[0] == 'n'){
      printf("Quitting...\n");
  }
  else{
      printf("Invalid input. Quitting without saving...\n");
  }
}

// Takes in the buffer and writes it into a file,
void writesave(char* buffer){

}
