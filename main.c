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
	if (argc != 2) {
		printf("argv[1] must indicate file name");
		exit(1);
	}
	
	int c;
	int x = 0;
  	int y = 1;
	int height;
  	int width;
	char *filename = argv[1];
	FILE *file = myopen(filename);

	struct file_buffer *file_buff = create_file_buffer(10);
	read_into_buffer(file, file_buff);

	initscr();
	raw();
	noecho();

	getmaxyx(stdscr, height, width);
	WINDOW *win = newwin(height, width, 0, 0);
	keypad(win, TRUE);

	mvwprintw(win,0,0, "Ctrl+Q - Exit\n");
	for (int r = 0; r < file_buff->rows; r++) {
		wprintw(win,"%s",file_buff->buffer[r]);
	}
	wrefresh(win);
	x = getcurx(win);
	y = getcury(win); 
	wmove(win, y, x);
	wrefresh(win);
	insert_row(file_buff,y-1);
	int xLineEnd = x;
	int yLineEnd = y;

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
		if (y-1 == file_buff->rows){
			xLineEnd = strlen(file_buff->buffer[y-1]);
		}
		else{
			xLineEnd = strlen(file_buff->buffer[y-1])-1;
		}
		if (c == 17){
			quit(file_buff);
			break;
		}
		if (c == KEY_LEFT){
			if (x > 0){
				x--;
			}
		}
		if (c == KEY_RIGHT){
			if (x < xLineEnd){
				x++;
			}
		}
		if (c == KEY_UP){
			if (y > 1){
				y--;
				if (y-1 == file_buff->rows){
					if (x > strlen(file_buff->buffer[y-1])){
						x = strlen(file_buff->buffer[y-1]);
						wmove(win, y, x);
						wrefresh(win);
					}
				}
				else{
					if (x > strlen(file_buff->buffer[y-1]-1)){
						x = strlen(file_buff->buffer[y-1])-1;
						wmove(win, y, x);
						wrefresh(win);
					}
				}
			}
		}
		if (c == KEY_DOWN){
			if (y < yLineEnd){
				y++;
				if (y-1 == file_buff->rows){
					if (x > strlen(file_buff->buffer[y-1])){
						x = strlen(file_buff->buffer[y-1]);
						xLineEnd = x;
						wmove(win, y, x);
						wrefresh(win);
					}
				}
				else{
					if (x > strlen(file_buff->buffer[y-1]-1)){
						x = strlen(file_buff->buffer[y-1])-1;
						xLineEnd = x;
						wmove(win, y, x);
						wrefresh(win);
					}
				}
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
			yLineEnd++;
			x = 0;
			xLineEnd = 0;
		}
		if (c>=32 && c<=126){
			insert_char(file_buff,y-1,x,c);
			x++;
		}
	}
  return 0;
}
