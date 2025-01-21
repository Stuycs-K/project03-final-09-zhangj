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
#include "cursor.h"
#include "filebuffer.h"
#include "utils.h"

// Takes in the current x and decreases it by one, shifting the cursor to the left by one
int keyleft(int x){
    if (x > 0){
        x--;
	}
    return x;
}

// Takes in the current x and the length of the line of the current line and increases it by one, shifting the cursor to the right by one
int keyright(int x, int xLineEnd){
    if (x < xLineEnd){ 
        x++;
    }
    return x;
}

// Takes in the current x, y, length of the line, and cursor y position and decreases y and cursor y by one, shifting the cursor up by one
int keyup(int* x, int y, int lineLen, int* curY){
    if (y > 1){
        y--;
        *curY-=1;
        if (*x > lineLen){ // Snaps cursor to end of line if cursor x position was greater in below line
            *x = lineLen;
        }
    }
    return y;
}

// Takes in the current x, y, length of the file buffer, number of rows in the file buffer, length of the line, 
// and cursor y position and increases y and cursor y by one, shifting the cursor down by one
int keydown(int* x, int y, int yLineEnd, int numrows, int linelen, int* curY){
    if (y < yLineEnd){
        y++;
        *curY+=1;
        if (y == numrows){ // Snaps cursor to end of line if cursor x position was greater in above line
            if (*x > linelen){
                *x = linelen;
            }
        }
        else{
            if (*x > linelen-1){
                *x = linelen-1;
            }
        }
    }
    return y;
}

// depending on keyboard input (left, right, up, down), moves the cursor 
// uses the other key(direction) functions defined above
void move_cursor(int c, int *x, int *y, int *curY, int xLineEnd, int yLineEnd, struct file_buffer *file_buff) {
	if (c == KEY_LEFT){
		*x = keyleft(*x);
	}
	if (c == KEY_RIGHT){
		*x = keyright(*x, xLineEnd);
	}
	if (c == KEY_UP && (*y) > 1){
		*y = keyup(x, *y, strlen(file_buff->buffer[(*y)-2])-1, curY);
	}
	if (c == KEY_DOWN){
		*y = keydown(x, *y, yLineEnd, file_buff->rows, strlen(file_buff->buffer[*y]), curY);
	}
}

// gets input from user and moves the cursor there if possible
// sets show_message and message if input was bad
void do_goto(WINDOW *win, int height, int width, int *x, int *y, int *show_message, struct file_buffer *file_buff, char *message) {
	clear_fgets_line(win, height, width);
	mvwprintw(win, height-BOTTOM_OFFSET, 0, "Go to line: ");
	wrefresh(win);
	char* line = malloc(256 * sizeof(char));
	my_fgets(win, line, height, '0', '9', 12); // custom fgets for window
	int lineNum = atoi(line);
	
	if (strcmp(line, "") == 0) {
		*show_message = 1;
		sprintf(message, "Error: cannot use empty string as line number.");
	}
	else if (!(1 <= lineNum && lineNum <= file_buff->rows)) {
		*show_message = 1;
		sprintf(message, "Error: cannot goto line #%d", lineNum);
	} 
	else {
		(*y) = lineNum;
		*x = strlen(file_buff->buffer[lineNum-1])-1;
	}
	free(line);
}