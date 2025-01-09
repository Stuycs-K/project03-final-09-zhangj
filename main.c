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
#include "undoredo.h"
#include "cutpaste.h"
#include "writesave.h"
#include "statdisplay.h"
#include "filehandle.h"

// Sighandler to catch all signals when using the text editor, run special actions depending on what signal is caught
static void sighandler(int signo){
}

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

// Main function for the text editor, parses arg for file name, runs text editor accordingly
int main(int argc, char * argv[]){
	// char *filename = "test.txt";
	// int fd = myopen(filename);

	printf("hello from the main femto\n");
	initscr();
	raw();
	noecho();
	int c;
	int x = 1;
  	int y = 2;
	int height;
  	int width;
	getmaxyx(stdscr, height, width);
	WINDOW *win = newwin(height, width, 0, 0);
	keypad(win, TRUE);
	// wprintw(win, "Ctrl+Q - Exit\n");
	// wrefresh(win);
	wmove(win, y, x);
	while (1) {
		c = wgetch(win);
		// printw("%c", c);
		// wrefresh(win);
		// if (c == 13){
		// 	printw("\n");
		// 	wrefresh(win);
		// }
		if (c == 17){
			quit();
			break;
		}
		if (x > 0 && c == KEY_LEFT){
			x--;
		}
		if (x < width-1 && c == KEY_RIGHT){
			x++;
		}
		if (y > 0 && c == KEY_UP){
			y--;
		}
		if (y < height-1 && c == KEY_DOWN){
			y++;
		}
		wmove(win, y, x);
		wrefresh(win);
	}
  return 0;
}
