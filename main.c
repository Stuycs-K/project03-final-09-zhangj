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
	// note: x is 0-indexed and y is 1-indexed
	int x, y;
	
	// the position of the on-screen cursor (note that the cursor's x-position always exactly equals its position in the buffer)
	int curY;
	
	// the length of the display for line numbers
	// able to be modified for tab display
	int offset;
	
	// boolean variables, described in their own sections
	int changed = 0, show_message = 0, longLine = 0;
	
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
	
	// these clamp x and y depending the strlen of the line and the size of the buffer
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
		curY = (y-1) % (height-TOTAL_OFFSET) + 1;
		fprintf(stderr, "height: %d; y: %d; curY: %d\n", height, y, curY);
		
		// get ready to display everything to the window
		wclear(win);
		wrefresh(win);
		
		// display messages for cursor, controls, and file information
		mvwprintw(win, 0, 0, TOP_DISPLAY_MESSAGE, y, x+1);
		mvwprintw(win, height-BOTTOM_OFFSET+1, 0, "%s", fileinfo);
		
		// display save message / error message if user hasn't inputted anything since the message has popped up
		if (show_message > 0) {
			clear_fgets_line(win, height, width);
			mvwprintw(win, height-BOTTOM_OFFSET, 0, "%s", message);
			show_message = 0;
		}
		
		// display the actual contents of the buffer
		for (int r = top; r < file_buff->rows && r+1 < bottom; r++) {
			mvwprintw(win, r-top+1, 0, "%03d| ", r+1);
			wprintw(win,"%s",file_buff->buffer[r]);
		}
		
		// calcuate the x offset based on tabs
		offset = OFFSET_INIT;
		for (int i = 0; i < x; i++){
			if (file_buff->buffer[y-1][i] == '\t') {
				offset += 8-(offset%8);
			} else {
				offset++;
			}
		}
		offset -= x + OFFSET_INIT;
		
		// move the cursor for actual input from user
		wmove(win, curY, x+offset+OFFSET_INIT);
		wrefresh(win);
		
		// the program blocks on this, which reads one character from stdin at a time
		c = wgetch(win);
		
		// set xLineEnd to be the length of the current line, -1 if not on last line
		xLineEnd = strlen(file_buff->buffer[y-1]);
		if (y != file_buff->rows) {
			xLineEnd--;
		}
		
		// QUIT
		if (c == to_ctrl_char('Q')) {
			quit(file_buff, filename, changed);
			free(filename);
			break;
		}
		
		// SAVE
		else if (c == to_ctrl_char('S')) {
			int can_save = check_can_save(win, file_buff, &filename, &show_message, message, height, width);
			if (can_save) {
				do_save(&file_buff, filename, &file, width, y, fileinfo, message);
				show_message = 1;
				changed = 0;
			}
		}
		
		// EXECUTE
		if (c == to_ctrl_char('T')) {
			prep_exec(win, height, width, command_line, arg_array);
			do_exec(file_buff, arg_array, message, &x, &y, &height, &width, &curY, &yLineEnd, &top, &bottom, &show_message, &changed);
		}
		
		// GOTO LINE
		if (c == to_ctrl_char('G')){
			do_goto(win, height, width, &x, &y, &show_message, file_buff, message);
		}
		
		// ARROW KEYS
		if (c == KEY_LEFT || c == KEY_RIGHT || c == KEY_UP || c == KEY_DOWN) {
			move_cursor(c, &x, &y, &curY, xLineEnd, yLineEnd, file_buff);
		} 
		
		// ALL OTHER CHARACTERS
		
		// DELETION
		if (c == KEY_BACKSPACE || c == KEY_DC || c == 127){
			changed = 1;
			do_delete(file_buff, &x, &y, &curY, &yLineEnd, &longLine);
		}
		
		// NEWLINE AND TABS
		if (c == '\n' || c == KEY_STAB || c == '\t') {
			changed = 1;
			do_insert_special(c, file_buff, &x, &y, &curY, &xLineEnd, &yLineEnd, &longLine, &show_message, message, offset, width);
		}
		
		// REGULAR CHARACTERS
		if (32 <= c && c <= 126) { // alphanumerics, punctuation, etc.
			changed = 1;
			do_insert_regular(c, file_buff, &x, &y, &curY, &xLineEnd, &yLineEnd, &longLine, &show_message, message, offset, width);
		}
	}
  return 0;
}
