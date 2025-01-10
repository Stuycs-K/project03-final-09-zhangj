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
	// // add later: if argc == 1: create the file later and ask when save/quit
	//
	// if (argc != 2) {
	// 	printf("argv[1] must indicate file name");
	// 	exit(1);
	// }
	// char *filename = argv[1];
	// FILE *file = myopen(filename);
	//
	// // 5 rows, 100 characters per row to start
	// int array_length = 5; // need to keep track of this manually for heap-allocated memory
	// char **buffer = init_2D_buffer(array_length, LINE_SIZE);
	// array_length = read_into_buffer(file, buffer, array_length);
	//
	// printf("buffer:\n");
	// showall(buffer, array_length);
	//
	// printf("end of buffer\n");

	// if (argc != 2) {
	// 	printf("argv[1] must indicate file name");
	// 	exit(1);
	// }
	
	// char *filename = argv[1];
	// FILE *file = myopen(filename);
	
	// struct file_buffer file_buff = create_file_buffer(10);
	// read_into_buffer(file, &file_buff);
	// showall(file_buff);

	// printf("hello from the main femto\n");
	initscr();
	raw();
	noecho();
	int c;
	int x = 0;
  	int y = 1;
	int height;
  	int width;
	getmaxyx(stdscr, height, width);
	WINDOW *win = newwin(height, width, 0, 0);
	keypad(win, TRUE);
	wprintw(win, "Ctrl+Q - Exit\n");
	wmove(win, y, x);
	wrefresh(win);
	char test[256];
	while (1) {
		for (int i = 0; i<256; i++){
			if (test[i] != 0){
				wprintw(win,"%c", test[i]);
			}
		}
		wrefresh(win);
		c = wgetch(win);
		if (c == 17){
			quit();
			break;
		}
		else if (c == KEY_LEFT){
			if (x > 0){
				x--;
			}
		}
		else if (c == KEY_RIGHT){
			if (x < width-1){
				x++;
			}
		}
		else if (c == KEY_UP){
			if (y > 1){
				y--;
			}
		}
		else if (c == KEY_DOWN){
			if (y < height-1){
				y++;
			}
		}
		else if (c == 127){
			test[x] = 0;
			x--;
		}
		else if (c == '\n'){
			wprintw(win,"\n");
			y++;
			x = 0;
			wrefresh(win);
		}
		else if (c>=32 && c<=126){
			test[x] = c;
			x++;	
		}
		wmove(win, y, x);
		wrefresh(win);
	}
  return 0;
}
