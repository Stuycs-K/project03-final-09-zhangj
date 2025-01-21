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

// to display the cursor position and controls
#define TOP_OFFSET 1
// to display the file information and asking for prompts / showing error messages
#define BOTTOM_OFFSET 2

#define TOP_DISPLAY_MESSAGE "%d:%d| Ctrl+Q - Quit  Ctrl+S - Save  Ctrl+T - Execute  Ctrl+G - Go to line #\n"

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
	int changed = 0, has_error = 0, longLine = 0;
	
	// stat info string, e.g. "test.txt: 10 bytes; last modified ..."
	char *fileinfo = (char*) calloc(LINE_SIZE, sizeof(char));
	
	// the name of the file. this may be initialized to UNTITLED_FILENAME, which indicates 
	// that the program has not been saved yet (and was ran with no args)
	char *filename = malloc(256 * sizeof(char));

	// used for ctrl+t (execute)
	char *command_line = (char*) malloc(LINE_SIZE * sizeof(char));
	char **arg_array = (char**) malloc(ARRAY_SIZE * sizeof(char*));
	
	// used to display a message when the user either saves, or incorrectly enters something else
	char *message = malloc(LINE_SIZE * sizeof(char));

	// creates a "struct file_buffer", described in filebuffer.h
	struct file_buffer *file_buff = create_file_buffer(10);
	FILE *file = init_file(argc,argv,filename,fileinfo);
	
	// initilization of an ncurses window
	initscr();
	raw();
	noecho();
	
	// height and width of the terminal screen
	int height, width;
	getmaxyx(stdscr, height, width);
	
	// initializes a WINDOW* pointer, which is used for all display and cursor movement
	WINDOW *win = newwin(height, width, 0, 0);
	// allows for extra input, like ctrl+char
	keypad(win, TRUE);
	
	// the bounds of which the filebuffer is shown to
	// changes when scrolling
	int top = 0, bottom = height - 2;
	
	// if the user gave a filename as an arg, read from that file into the buffer
	if (argc == 2){
		read_into_buffer(file, file_buff, width);
	}
	
	// appends an extra newline to the file_buffer at initilization, the program segfaults without it
	if (argc == 2 && file_buff->rows > 1) {
		int length = strlen(file_buff->buffer[file_buff->rows-1]);
		if (file_buff->buffer[file_buff->rows-1][length-1] == '\n') {
			insert_row(file_buff, file_buff->rows);
		}
	} else if (argc == 1 || file_buff->rows == 0){
		insert_row(file_buff, 0);
	}
	
	// initialize x and y to be at the end of the buffer
	y = file_buff->rows;
	x = strlen(file_buff->buffer[y-1]);
	curY = y;
	
	// ??? clamps for x and y
	int xLineEnd = x;
	int yLineEnd = y;

	// main loop of the program
	while (1) {
		// check if the termianl was resized
		// doesn't always help but sometimes works to resize things
		getmaxyx(win, height, width);
		
		// change top and bottom if the cursor was moved to switch windows
		while (y >= bottom){
			bottom += height-3;
			top += height-3;
		}
		while (y <= top && top > 0){
			bottom -= height-3;
			top -= height-3;
		}
		// put the cursor in the right spot
		curY = y % (height-3);
		
		// display everything to the window
		wclear(win);
		wrefresh(win);
		
		// display messages for cursor, controls, and file information
		mvwprintw(win, 0, 0, TOP_DISPLAY_MESSAGE, y, x+1);
		mvwprintw(win, height-1, 0, "%s", fileinfo);
		
		// display save message / error message if user hasn't inputted anything since the message has popped up
		if (has_error > 0) {
			clear_fgets_line(win, height, width);
			mvwprintw(win, height-2, 0, "%s", message);
			has_error = 0;
		}
		
		// display the actual contents of the buffer
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
					sprintf(message, "Error: cannot use empty string as filename.");
					can_save = 0;
				}
				else if (strcmp(line, UNTITLED_FILENAME) == 0) {
					has_error = 1;
					sprintf(message, "Error: cannot use %s as filename.", UNTITLED_FILENAME);
					can_save = 0;
				} else if (access(line, F_OK) != -1){  // check if the file exists using access()
					char *secondline = (char*) malloc(LINE_SIZE * sizeof(char));
					clear_fgets_line(win, height, width);
					wmove(win, height-2, 0);
					wprintw(win, "The file \"%s\" already exists. Overwrite it? (y/n): ", line);
					wrefresh(win);
					my_fgets(win, secondline, height, 32, 126, 23);
					if (secondline[0] == '\0') {
						sprintf(message, "Error: no response to y/n.");
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
						sprintf(message, "Error: did not recognize response '%s'. Not saving.", secondline);
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
				sprintf(message, "File saved.");
				has_error = 1;
				changed = 0;

			}
		}
		if (c == to_ctrl_char('T')) {
			clear_fgets_line(win, height, width);
			wmove(win, height-2, 0);
			wprintw(win, "Enter Command: ");
			wrefresh(win);
			
			my_fgets(win, command_line, height, 32, 126, 15);
			int command_line_length = strlen(command_line);
			if (command_line[command_line_length-1] == '\n') { // strip newline
				command_line[command_line_length-1] = '\0';
			}
			parse_args(command_line, arg_array);

			do_exec(file_buff, arg_array, message, &x, &y, &height, &width, &curY, &yLineEnd, &top, &bottom, &has_error, &changed);
		}
		if (c == to_ctrl_char('G')){
			clear_fgets_line(win, height, width);
			wmove(win, height-2, 0);
			wprintw(win, "Go to line: ");
			wrefresh(win);
			char* line = malloc(256 * sizeof(char));
			my_fgets(win, line, height, '0', '9', 12); // custom fgets for window
			int lineNum = atoi(line);
			
			if (strcmp(line, "") == 0) {
				has_error = 1;
				sprintf(message, "Error: cannot use empty string as line number.");
			}
			else if (!(1 <= lineNum && lineNum <= file_buff->rows)) {
				has_error = 1;
				sprintf(message, "Error: cannot goto line #%d", lineNum);
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
				sprintf(message, "Error: Tabs are not supported with long lines.");
			}
			else if (x+offset+8-(offset%8)<width-7){
				insert_char(file_buff,y-1,x,'\t');
				x++;
			}
			else{
				has_error = 1;
				sprintf(message, "Error: You may not tab past the window length.");
			}
		}
		if (32 <= c && c <= 126) { // alphanumerics, punctuation, etc.
			changed = 1;
			if (longLine == 1 && x+offset>=width-7){
				has_error = 1;
				sprintf(message, "Error: Maximum line length is 2x the window width.");
			}
			else if (x+offset>=width-7){
				if (offset > 0){
					has_error = 1;
					sprintf(message, "Error: Tabs are not supported with long lines.");
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
