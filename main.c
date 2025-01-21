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
#include <sys/wait.h>
#include "writesave.h"
#include "statdisplay.h"
#include "filebuffer.h"
#include "cursor.h"
#include "execprint.h"
#include "filehandle.h"

// Group 20

static void signal_handler(int signo) {
	if (signo == SIGSEGV) {
		endwin();
		printf("Program Segfaulted\n");
		exit(1);
	}
}

// ascii values 1-26 are ctrl + ch (ctrl A is 1)
int to_ctrl_char(int ch) {
	if (islower(ch)) { ch = toupper(ch); }
	return ch - 'A' + 1;
}

void clear_fgets_line(WINDOW *win, int height, int width) {
	wmove(win, height-2, 0);
	for (int i = 0; i < width; i++)
		wprintw(win, " ");
}

void my_fgets(WINDOW *win, char *line, int height, int char_range_min, int char_range_max, int promptLen) {
	int ind = 0;
	int c1;
	char lineBuff[255];

	while (1){
		c1 = wgetch(win);
		if (c1 == '\n'){
			line[ind] = '\0';
			return;
		}
		if (c1 == KEY_BACKSPACE || c1 == KEY_DC || c1 == 127){
			if (getcurx(win) > promptLen){
				line[ind] = '\0';
				ind--;
				wmove(win, height-2, getcurx(win)-1);
				wclrtoeol(win);
				wrefresh(win);
			}
			else{
				line[0] = '\0';
			}
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
	int c, x = 0, y = 0, height, width, taboffset = 5, saved = 0, changed = 0, top = 0, lineNum, has_error=0, longLine = 0;
	char *fileinfo = (char*) calloc(LINE_SIZE, sizeof(char));

	FILE *file;
	char* filename = malloc(256 * sizeof(char));
	int pipe_fds[2];

	char* cmd_args = malloc(LINE_SIZE * sizeof(char));
	char **arg_array = (char**) malloc(ARRAY_SIZE * sizeof(char*));

	fileinfo = (char*) calloc(LINE_SIZE, sizeof(char));
	
	struct file_buffer *file_buff = create_file_buffer(10);
	file = initFile(argc,argv,filename,fileinfo);

	initscr();
	raw();
	noecho();
	getmaxyx(stdscr, height, width);
	int bottom = height-2;
	WINDOW *win = newwin(height, width, 0, 0);
	keypad(win, TRUE);

	if (argc == 2){
		read_into_buffer(file, file_buff, width);
		if (file_buff->rows > 1){
			int length = strlen(file_buff->buffer[file_buff->rows-1]);
			if (file_buff->buffer[file_buff->rows-1][length-1]=='\n'){
				insert_row(file_buff,file_buff->rows);
			}
		}
	}
	if (argc == 1 || file_buff->rows == 0){
		insert_row(file_buff,0);
	}
	y = file_buff->rows;
	x = strlen(file_buff->buffer[y-1]);
	int xLineEnd = x;
	int yLineEnd = y;
	int curY = y;

	char *error_message = malloc(LINE_SIZE * sizeof(char));
	
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
		mvwprintw(win,0,0,"%d:%d| %d Ctrl+Q - Exit  Ctrl+S - Save  Ctrl+T - Execute  Ctrl+G - Go to line #\n", y, x+1, curY);
		mvwprintw(win,height-1,0, "%s", fileinfo);
		if (saved > 0){
			mvwprintw(win, height-2, 0, "File Saved.");
			saved = 0;
		}
		else if (has_error > 0) {
			clear_fgets_line(win, height, width);
			mvwprintw(win, height-2, 0, "%s", error_message);
			has_error = 0;
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
		taboffset = 5;
		for (int i = 0; i<x; i++){
			if ((file_buff->buffer[y-1])[i] == '\t'){
				taboffset += 8-(taboffset%8);
			}
			else{
				taboffset++;
			}
		}
		taboffset-=x+5;
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
			int can_save = 1;
			if (strcmp(filename, UNTITLED_FILENAME) == 0){
				char* line = malloc(256 * sizeof(char));
				clear_fgets_line(win, height, width);
				wmove(win, height-2, 0);
				wprintw(win, "(Save) Enter Filename: ");
				wrefresh(win);
				
				my_fgets(win, line, height, 32, 126, 23);
				// check that:
				// - the filename is not empty and
				// - the filename is not "untitled.txt"
				if (strcmp(line, "") == 0) {
					has_error = 1;
					sprintf(error_message, "Error: cannot use empty string as filename.");
					can_save = 0;
				}
				else if (strcmp(line, UNTITLED_FILENAME) == 0) {
					has_error = 1;
					sprintf(error_message, "Error: cannot use %s as filename.", UNTITLED_FILENAME);
					can_save = 0;
				} else if (access(line, F_OK) != -1){  // check if the file exists using access()
					char *secondline = (char*) malloc(LINE_SIZE * sizeof(char));
					clear_fgets_line(win, height, width);
					wmove(win, height-2, 0);
					wprintw(win, "The file \"%s\" already exists. Overwrite it? (y/n): ", line);
					wrefresh(win);
					my_fgets(win, secondline, height, 32, 126, 23);
					if (secondline[0] == '\0') {
						sprintf(error_message, "Error: no response to y/n.");
						has_error = 1;
						can_save = 0;
					} else if (secondline[0] == 'y') {
						can_save = 1;
						free(filename);
						filename = line;
						remove(UNTITLED_FILENAME);
					} else if (secondline[0] == 'n') {
						can_save = 0;
					} else {
						sprintf(error_message, "Error: did not recognize response '%s'. Not saving.", secondline);
						has_error = 1;
						can_save = 0;
					}
				} else {
					free(filename);
					filename = line;
				}		
			}

			if (can_save) {
				save(file_buff, filename);
				free(file_buff);
				file_buff = create_file_buffer(10);
				file = open_read(filename);
				read_into_buffer(file, file_buff, width);
				if (y > file_buff->rows){
					insert_row(file_buff,y-1);
				}
				stat_info(filename, fileinfo);
				saved = 1;
				changed = 0;

			}
		}
		if (c == to_ctrl_char('T')) {
			clear_fgets_line(win, height, width);
			wmove(win, height-2, 0);
			wprintw(win, "Enter Command: ");
			wrefresh(win);
			
			my_fgets(win, cmd_args, height, 32, 126, 15);
			int cmd_args_length = strlen(cmd_args);
			if (cmd_args[cmd_args_length-1] == '\n') { // strip newline
				cmd_args[cmd_args_length-1] = '\0';
			}
			parse_args(cmd_args, arg_array);

			do_exec(file_buff, pipe_fds, arg_array, error_message, &x, &y, &height, &width, &curY, &yLineEnd, &top, &bottom, &has_error, &changed);
		}
		if (c == to_ctrl_char('G')){
			clear_fgets_line(win, height, width);
			wmove(win, height-2, 0);
			wprintw(win, "Go to line: ");
			wrefresh(win);
			char* line = malloc(256 * sizeof(char));
			my_fgets(win, line, height, '0', '9', 12); // custom fgets for window
			lineNum = atoi(line);
			
			if (strcmp(line, "") == 0) {
				has_error = 1;
				sprintf(error_message, "Error: cannot use empty string as line number.");
			}
			else if (!(1 <= lineNum && lineNum <= file_buff->rows)) {
				has_error = 1;
				sprintf(error_message, "Error: cannot goto line #%d", lineNum);
			} 
			else {
				
				if (lineNum <= file_buff->rows){
					y = lineNum;
					curY = lineNum;
	        while (lineNum < top){
	          top -= height-3;
	          bottom -= height-3;
						curY -= height-3;
	        }
	        while (lineNum > bottom){
	          top += height-3;
	          bottom += height-3;
						curY -= height-3;
	        }
					x = strlen(file_buff->buffer[lineNum-1])-1;
				}
				
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
				if (longLine == 1){
					delete_char(file_buff,y-1,x-1);
					x--;
					longLine = 0;
				}
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
			longLine = 0;
		}
		if (c == KEY_STAB || c=='\t'){
			changed = 1;
			if (longLine == 1){
				has_error = 1;
				sprintf(error_message, "Error: Tabs are not supported with long lines.");
			}
			else if (x+taboffset+8-(taboffset%8)<width-7){
				insert_char(file_buff,y-1,x,'\t');
				x++;
			}
			else{
				has_error = 1;
				sprintf(error_message, "Error: You may not tab past the window length.");
			}
		}
		if (32 <= c && c <= 126) { // alphanumerics, punctuation, etc.
			changed = 1;
			if (longLine == 1 && x+taboffset>=width-7){
				has_error = 1;
				sprintf(error_message, "Error: Maximum line length is 2x the window width.");
			}
			else if (x+taboffset>=width-7){
				if (taboffset > 0){
					has_error = 1;
					sprintf(error_message, "Error: Tabs are not supported with long lines.");
				}
				else{
					insert_char(file_buff,y-1,x,'-');
					insert_char(file_buff,y-1,x+1,'\n');
					insert_row(file_buff,y);
					y++;
					curY++;
					yLineEnd++;
					x = 0;
					xLineEnd = 0;
					longLine = 1;
					insert_char(file_buff,y-1,x,c);
					x++;
				}
			}
			else{
				insert_char(file_buff,y-1,x,c);
				x++;
			}
		}
	}
  return 0;
}
