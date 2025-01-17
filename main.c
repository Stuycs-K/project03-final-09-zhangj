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

// Group 20

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
	int c, x = 0, y = 0, height, width, taboffset = 0, saved = 0, changed = 0, top = 0, lineNum, c1;
	char *filename, *fileinfo, *line = "";
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
	int curY = y;

	while (1) {
		getmaxyx(win, height, width);
		if (y >= bottom){
			bottom += height-3;
			top += height-3;
			curY = 1;
		}
		if (y <= top && top > 0){
			bottom -= height-3;
			top -= height-3;
			curY = height-3;
		}
		wclear(win);
		wrefresh(win);
		mvwprintw(win,0,0,"%d:%d | Ctrl+Q - Exit  Ctrl+S - Save\n", y, x+1);
		mvwprintw(win,height-1,0, "%s", fileinfo);
		if (saved > 0){
			mvwprintw(win, height-2, 0, "File Saved.");
			saved = 0;
		}
		for (int r = top+1; r < file_buff->rows+1; r++){
			if (r >= bottom){
				break;
			}
			mvwprintw(win, r-top, 0, "%d| ", r);
		}
		for (int r = top; r < file_buff->rows; r++) {
			if (r > bottom-2){
				break;
			}
			mvwprintw(win,r-top+1,2+numDigits(r+1),"%s",file_buff->buffer[r]);
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
		wmove(win, curY, x+taboffset+numDigits(y)+2);
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
		if (c == to_ctrl_char('G')){
			strcpy(line,"");
			char lineBuff[8];
			mvwprintw(win, height-2, 0, "Go to line: ");
			wmove(win, height-2, 12);
			wrefresh(win);
			// while (1){
			// 	c1 = wgetch(win);
			// 	if (c1 == '\n'){
			// 		break;
			// 	}
			// 	if (c >= 48 && c <= 57){
			// 		sprintf(lineBuff, "%d", c1);
			// 		strcat(line,&lineBuff[0]);
			// 		wprintw(win,"%c", lineBuff[0]);
			// 	}
			// }
			// lineNum = atoi(line);
			// if (lineNum < file_buff->rows){
			// 	y = lineNum;
			// 	curY = lineNum;
			// 	x = strlen(file_buff->buffer[y-1]);
			// }
		}
		if (c == KEY_LEFT){
			x = keyleft(x, y);
		}
		if (c == KEY_RIGHT){
			x = keyright(x, xLineEnd);
		}
		if (c == KEY_UP){
			y = keyup(&x, y, strlen(file_buff->buffer[y-2])-1, &curY);
		}
		if (c == KEY_DOWN){
			y = keydown(&x, y, yLineEnd, file_buff->rows, strlen(file_buff->buffer[y]), &curY);
		}
		if (c == KEY_BACKSPACE || c == KEY_DC || c == 127){
			changed = 1;
			if (x == 0 && y > 1){
				delete_row(file_buff, y-1);
				y--;
				curY--;
				yLineEnd = y;
				x = strlen(file_buff->buffer[y-1]);
				delete_char(file_buff,y-1,x-1);
				x--;
			}
			else if (x > 0){
				delete_char(file_buff,y-1,x-1);
				x--;
			}
		}
		if (c == '\n'){
			changed = 1;
			insert_char(file_buff,y-1,x,'\n');
			insert_row(file_buff,y);
			y++;
			curY++;
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
			if (x+taboffset>=width-5){
				insert_char(file_buff,y-1,x,'-');
				insert_char(file_buff,y-1,x+1,'\n');
				insert_row(file_buff,y);
				y++;
				curY++;
				yLineEnd++;
				x = 0;
				xLineEnd = 0;
			}
			else{
				insert_char(file_buff,y-1,x,c);
				x++;
			}
		}
	}
    return 0;
}
