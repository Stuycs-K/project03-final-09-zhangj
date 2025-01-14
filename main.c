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
	int c;
	int x = 0;
	int y = 1;
	int height;
	int width;
	int taboffset = 0;
	char *filename;
	FILE *file ;
	char *fileinfo;

	if (argc == 1){
		filename = "Untitled.txt";
		file = fopen(filename, "w+");
		close_file(file);
		fileinfo = stat_info(filename);
	}
	else if (argc == 2){
		filename = argv[1];
		file = open_read(filename);
		fileinfo = stat_info(argv[1]); // later: update this whenever user saves
	}
	else {
		printf("Incorrect number of arguments");
		exit(1);
	}

	struct file_buffer *file_buff = create_file_buffer(10);
	if (argc == 2){
		read_into_buffer(file, file_buff);
	}

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
		wmove(win, height-1, 0);
		wprintw(win, "%s", fileinfo);
		wmove(win, 1, 0);
		for (int r = 0; r < file_buff->rows; r++) {
			wprintw(win,"%s",file_buff->buffer[r]);
		}
		taboffset = 0;
		for (int i = 0; i<x; i++){
			if ((file_buff->buffer[y-1])[i] == '\t'){
				taboffset += 8-(taboffset%8);
			}
			else{
				taboffset++;
			}
		}
		taboffset-=x;
		wmove(win, y, x+taboffset);
		wrefresh(win);
		c = wgetch(win);
		if (y == file_buff->rows){
			xLineEnd = strlen(file_buff->buffer[y-1]);
		}
		else{
			xLineEnd = strlen(file_buff->buffer[y-1])-1;
		}
		if (c == 17){
			quit(file_buff, filename);
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
				if (x > strlen(file_buff->buffer[y-1])-1){
					x = strlen(file_buff->buffer[y-1])-1;
				}
			}
		}
		if (c == KEY_DOWN){
			if (y < yLineEnd){
				y++;
				if (y == file_buff->rows){
					if (x > strlen(file_buff->buffer[y-1])){
						x = strlen(file_buff->buffer[y-1]);
					}
				}
				else{
					if (x > strlen(file_buff->buffer[y-1])-1){
						x = strlen(file_buff->buffer[y-1])-1;
					}
				}
			}
		}
		if (c == KEY_BACKSPACE || c == KEY_DC || c == 127){
			if (x == 0){
				delete_row(file_buff, y-1);
				y--;
				yLineEnd = y;
				x = strlen(file_buff->buffer[y-1]);
				delete_char(file_buff,y-1,x-1);
				x--;
			}
			else{
				delete_char(file_buff,y-1,x-1);
				x--;
			}
		}
		if (c == '\n'){
			insert_char(file_buff,y-1,x,'\n');
			insert_row(file_buff,y);
			y++;
			yLineEnd++;
			x = 0;
			xLineEnd = 0;
		}
		if (c == KEY_STAB || c == 9 || c=='\t'){
			insert_char(file_buff,y-1,x,'\t');
			x++;
		}
		if (c>=32 && c<=126){
			if (x+taboffset>=width-1){
				insert_char(file_buff,y-1,x,'\n');
				insert_row(file_buff,y);
				y++;
				yLineEnd++;
				x = 0;
				xLineEnd = 0;
			}
			insert_char(file_buff,y-1,x,c);
			x++;
		}
	}
  return 0;
}
