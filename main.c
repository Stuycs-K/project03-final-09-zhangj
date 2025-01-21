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
#include "utils.h"

// allows the program to exist gracefully on the occasional segfault
static void signal_handler(int signo) {
	if (signo == SIGSEGV) {
		endwin();
		printf("Program Segfaulted\n");
		exit(1);
	}
}

// Main function for the text editor, parses arg for file name, runs text editor accordingly
int main(int argc, char *argv[]) {
	signal(SIGSEGV, signal_handler);
	
	// char, allows for more than just ascii input (e.g. arrows keys)
	int c;
	
	// the position of the file buffer's cursor, not the actual cursor
	int x, y;
	
	// the position of the on-screen cursor (note that the cursor's x-position always exactly equals its position in the buffer)
	int curY;
	
	// offset, initially 5, the length of the display for line numbers
	// able to be modified for tab display
	int offset = 5;
	
	// boolean variables, described in their own sections
	int saved = 0, changed = 0, has_error = 0, longLine = 0;
	
	int top = 0;
	
	int lineNum = 0;
	
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
	
	// height and width of the terminal screen
	int height, width;
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
	curY = y;

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
		mvwprintw(win,0,0,"%d:%d| Ctrl+Q - Quit  Ctrl+S - Save  Ctrl+T - Execute  Ctrl+G - Go to line #\n", y, x+1);
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

		for (int r = top; r < file_buff->rows; r++) {
			if (r+1 >= bottom){
				break;
			}
			mvwprintw(win, r-top+1, 0, "%03d| ", r+1);
			wprintw(win,"%s",file_buff->buffer[r]);
		}
		offset = 5;
		for (int i = 0; i<x; i++){
			if ((file_buff->buffer[y-1])[i] == '\t'){
				offset += 8-(offset%8);
			}
			else{
				offset++;
			}
		}
		offset-=x+5;
		wmove(win, curY, x+offset+5);
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
					y = lineNum;
					curY = y % (height - 3);
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
			else if (x+offset+8-(offset%8)<width-7){
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
			if (longLine == 1 && x+offset>=width-7){
				has_error = 1;
				sprintf(error_message, "Error: Maximum line length is 2x the window width.");
			}
			else if (x+offset>=width-7){
				if (offset > 0){
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
