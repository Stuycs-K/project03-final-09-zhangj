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
#include <ctype.h>
#include <ncurses.h>
#include "cutpaste.h"
#include "writesave.h"
#include "statdisplay.h"
#include "filehandle.h"
#include "cursor.h"

void signal_handler() {
  endwin();
  printf("Segfault\n");
  exit(1);
} 

// ascii values 1-26 are ctrl + ch (ctrl A is 1)
int to_ctrl_char(int ch) {
	if (islower(ch)) { ch = toupper(ch); }
	return ch - 'A' + 1;
}

// Main function for the text editor, parses arg for file name, runs text editor accordingly
int main(int argc, char *argv[]) {
	signal(SIGSEGV, signal_handler);
	int c, x = 0, y = 0, height, width, taboffset = 0, saved = 0, changed = 0, top = 0;
	char *filename, *fileinfo;
	FILE *file;

	fileinfo = (char*) calloc(LINE_SIZE, sizeof(char));

	if (argc == 1){
		filename = "Untitled.txt";
		file = fopen(filename, "w+");
		close_file(file);
		stat_info(filename, fileinfo);
	}
	else if (argc == 2){
		filename = argv[1];
		file = open_read(filename);
		fileinfo = stat_info(argv[1], fileinfo);
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
	int bottom = height-2;
	WINDOW *win = newwin(height, width, 0, 0);
	keypad(win, TRUE);

	wmove(win,1,0);
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
		getmaxyx(win, height, width);
		if (y >= bottom){
			bottom += y;
			top = y;
		}
		wclear(win);
		wrefresh(win);
		mvwprintw(win,0,0,"Ctrl+Q - Exit  Ctrl+S - Save\n");
		mvwprintw(win,height-1,0, "%s", fileinfo);
		if (saved > 0){
			mvwprintw(win, height-2, 0, "File Saved.");
			saved = 0;
		}
		wmove(win, 1, 0);
		wrefresh(win);
		for (int r = top; r < bottom; r++) {
			if (strlen(file_buff->buffer)==0){
				break;
			}
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
		c = wgetch(win); // program waits on this
		if (y == file_buff->rows){
			xLineEnd = strlen(file_buff->buffer[y-1]);
		}
		else{
			xLineEnd = strlen(file_buff->buffer[y-1])-1;
		}
		if (c == to_ctrl_char('Q')) {
			quit(file_buff, filename, changed);
			break;
		}
		if (c == to_ctrl_char('S')) {
			save(file_buff, filename);
			stat_info(filename, fileinfo);
			struct file_buffer *file_buff = create_file_buffer(10);
			saved = 1;
			if (strcmp(filename,"Untitled.txt") != 0){
				changed = 0;
			}
		}
		if (c == to_ctrl_char('C')) {
			// copy
		}
		if (c == to_ctrl_char('V')) {
			// paste
		}
		if (c == to_ctrl_char('X')) {
			// cut (copy and remove)
		}
		if (c == to_ctrl_char('T')) {
			// execute
		}

		if (c == KEY_LEFT){
			x = keyleft(x);
		}
		if (c == KEY_RIGHT){
			x = keyright(x, xLineEnd);
		}
		if (c == KEY_UP){
			y = keyup(&x, y, strlen(file_buff->buffer[y-2])-1);
		}
		if (c == KEY_DOWN){
			y = keydown(&x, y, yLineEnd, file_buff->rows, strlen(file_buff->buffer[y]));
		}
		if (c == KEY_BACKSPACE || c == KEY_DC || c == 127){
			changed = 1;
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
			changed = 1;
			insert_char(file_buff,y-1,x,'\n');
			insert_row(file_buff,y);
			y++;
			yLineEnd++;
			x = 0;
			xLineEnd = 0;
		}
		if (c == KEY_STAB || c=='\t'){
			changed = 1;
			if (x+taboffset+8-(taboffset%8)<width-1){
				insert_char(file_buff,y-1,x,'\t');
				x++;
			}
		}
		if (32 <= c && c <= 126) { // alphanumerics, punctuation, etc.
			changed = 1;
			if (x+taboffset>=width-1){
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
