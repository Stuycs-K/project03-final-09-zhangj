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
int main(int argc, char *argv[]) {
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
	wmove(win, y, x);
	wrefresh(win);
	struct file_buffer *file_buff = create_file_buffer(10);
	insert_row(file_buff,0);
	mvwprintw(win,0,0, "Ctrl+Q - Exit\n");
	wrefresh(win);
	while (1) {
		wclear(win);
		wrefresh(win);
		mvwprintw(win,0,0, "Ctrl+Q - Exit\n");
		for (int r = 0; r < file_buff->rows; r++) {
			wprintw(win,"%s",file_buff->buffer[r]);
		}
		wmove(win, y, x);
		wrefresh(win);
		c = wgetch(win);
		wrefresh(win);
		if (c == 17){
			quit();
			break;
		}
		if (c == KEY_LEFT){
			if (x > 0){
				x--;
			}
		}
		if (c == KEY_RIGHT){
			if (x < width-1){
				x++;
			}
		}
		if (c == KEY_UP){
			if (y > 1){
				y--;
			}
		}
		if (c == KEY_DOWN){
			if (y < height-1){
				y++;
			}
		}
		// else if (c == KEY_BACKSPACE || c == KEY_DC || c == 127){
		// 	test[x-1] = '\0';
		// 	x--;
		// }
		if (c == '\n'){
			insert_char(file_buff,y-1,x,'\n');
			insert_row(file_buff,y);
			y++;
			x = 0;
		}
		if (c>=32 && c<=126){
			insert_char(file_buff,y-1,x,c);
			x++;
		}
	}
  return 0;
}
