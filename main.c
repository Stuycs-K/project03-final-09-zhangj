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
	initscr();
	raw();
	noecho();
	int c;
	int x = 0;
  	int y = 1;
	int height;
  	int width;
	getmaxyx(stdscr, height, width);
	WINDOW *win = newwin(height, width, 0, 0);
	keypad(win, TRUE);
	wprintw(win, "Ctrl+Q - Exit\n");
	wmove(win, y, x);
	wrefresh(win);
	while (1) {
		c = wgetch(win);
		if (c == 17){
			quit();
			break;
		}
		else if (x > 0 && c == KEY_LEFT){
			x--;
		}
		else if (x < width-1 && c == KEY_RIGHT){
			x++;
		}
		else if (y > 1 && c == KEY_UP){
			y--;
		}
		else if (y < height-1 && c == KEY_DOWN){
			y++;
		}
		else if (c == 13){
			wprintw(win,"\n");
			y++;
			x = 0;
			wrefresh(win);
		}
		else{
			wprintw(win,"%c", c);
			x++;
			wrefresh(win);
		}
		wmove(win, y, x);
		wrefresh(win);
	}
  return 0;
}
