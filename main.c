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

// Main function for the text editor, parses arg for file name, runs text editor accordingly
int main(int argc, char * argv[]){
	// char *filename = "test.txt";
	// int fd = myopen(filename);

	printf("hello from the main femto\n");
	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	printw("Ctrl+Q - Exit\n");
	refresh();
	int c;
	int x = 50;
  int y = 5;
	int height = 10;
  int width = 100;
  int starty = (LINES - height) / 2;
  int startx = (COLS - width) / 2;
	WINDOW *my_win;
	my_win = newwin(25, 50, y, x);
  wmove(my_win, y, x);
	while (1) {
			c = getch();
			if (c == 17){
				quit();
				break;
			}
			else{
				if (c == KEY_LEFT){
					 x--;
				 }
				if (c == KEY_RIGHT){
					 x++;
				 }
			  if (c == KEY_UP){
				  y--;
			  }
			  if (c == KEY_DOWN){
				  y++;
			  }
			  wmove(my_win, y, x);
			  wrefresh(my_win);
	 	}
	}
  return 0;
}
