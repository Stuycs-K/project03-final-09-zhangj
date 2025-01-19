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
#include "filebuffer.h"
#include "cursor.h"
#include "execprint.h"
#include "filehandle.c"

// Group 20

void signal_handler() {
  endwin();
  printf("Program Segfaulted\n");
  exit(1);
}

// ascii values 1-26 are ctrl + ch (ctrl A is 1)
int to_ctrl_char(int ch) {
	if (islower(ch)) { ch = toupper(ch); }
	return ch - 'A' + 1;
}

void my_fgets(WINDOW *win, char *line, int height, int char_range_min, int char_range_max) {
	int ind = 0;
	int c1;
	char lineBuff[255];
	
	while (1){
		c1 = wgetch(win);
		if (c1 == '\n'){
			return;
		}
		if (c1 == KEY_BACKSPACE || c1 == KEY_DC || c1 == 127){
			line[ind] = '\0';
			ind--;
			wmove(win, height-2, getcurx(win)-1);
			wclrtoeol(win);
			wrefresh(win);
		}
		if (char_range_min <= c1 && c1 <= char_range_max){
			sprintf(lineBuff, "%c", c1);
			line[ind] = lineBuff[0];
			wprintw(win,"%s", lineBuff);
			wrefresh(win);
			ind++;
		}
	}
}


// Main function for the text editor, parses arg for file name, runs text editor accordingly
int main(int argc, char *argv[]) {
	signal(SIGSEGV, signal_handler);
	int c, x = 0, y = 0, height, width, taboffset = 0, saved = 0, changed = 0, top = 0, lineNum;
	char *fileinfo = (char*) calloc(LINE_SIZE, sizeof(char));
	char* filename = malloc(256 * sizeof(char));
	FILE *file;

	initFile(argc,argv,filename,fileinfo,file);

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
	int xLineEnd = x;
	int yLineEnd = y;
	int curY = y;
  if (strcmp(filename,"Untitled.txt")==0){
    insert_row(file_buff,y-1);
  }

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
			mvwprintw(win, r-top, 0, "%03d| ", r);
		}
		for (int r = top; r < file_buff->rows; r++) {
			if (r > bottom-2){
				break;
			}
			mvwprintw(win,r-top+1,5,"%s",file_buff->buffer[r]);
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
		wmove(win, curY, x+taboffset+5);
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
			free(filename);
			break;
		}
		if (c == to_ctrl_char('S')) {
			if (strcmp(filename,"Untitled.txt") == 0){
				wmove(win, height-2, 0);
				wprintw(win, "(Save) Enter Filename: ");
				wrefresh(win);
				char* line = malloc(256 * sizeof(char));
				my_fgets(win, line, height, 32, 126);
				free(filename);
				filename = line;
			}
			save(file_buff, filename);
			stat_info(filename, fileinfo);
			saved = 1;
      changed = 0;
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
			wmove(win, height-2, 0);
			wprintw(win, "Enter Command: ");
			wrefresh(win);
			char* cmd_args = malloc(256 * sizeof(char));
			my_fgets(win, cmd_args, height, 32, 126);
			
			// Fork, pipe output of execvp to parent, copy output from pipe to file buffer
			// char * args[256];
			// parse_args(line, args);
			// execvp(args[0], args);
			// free(line);
		}
		if (c == to_ctrl_char('G')){
			wmove(win, height-2, 0);
			wprintw(win, "Go to line: ");
			wrefresh(win);
			char* line = malloc(256 * sizeof(char));
			my_fgets(win, line, height, '0', '9'); // custom fgets for window
			lineNum = atoi(line);
			
			if (lineNum <= file_buff->rows){
        while (lineNum < top){
          top -= height-3;
          bottom -= height-3;
        }
        while (lineNum > bottom){
          top += height-3;
          bottom += height-3;
        }
				y = lineNum;
				curY = lineNum;
				x = strlen(file_buff->buffer[lineNum-1])-1;
			}
			free(line);
		}
		if (c == KEY_LEFT){
			x = keyleft(x, y);
		}
		if (c == KEY_RIGHT){
			x = keyright(x, xLineEnd);
		}
		if (c == KEY_UP && y > 1){
			y = keyup(&x, y, strlen(file_buff->buffer[y-2])-1, &curY);
		}
		if (c == KEY_DOWN){
			y = keydown(&x, y, yLineEnd, file_buff->rows, strlen(file_buff->buffer[y]), &curY);
		}
		if (c == KEY_BACKSPACE || c == KEY_DC || c == 127){
			changed = 1;
			if (x == 0 && y > 1){
				int newX = strlen(file_buff->buffer[y-2])-1;
				delete_newline(file_buff, y-1);
				y--;
				x = newX;
				curY--;
				yLineEnd--;
			}
			else if (x > 0){
				delete_char(file_buff,y-1,x-1);
				x--;
			}
		}
		if (c == '\n'){
			changed = 1;
			insert_newline(file_buff, y-1, x);
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
