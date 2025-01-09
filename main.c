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
	// add later: if argc == 1: create the file later and ask when save/quit

	if (argc != 2) {
		printf("argv[1] must indicate file name");
		exit(1);
	}
	char *filename = argv[1];
	FILE *file = myopen(filename);

	// 5 rows, 100 characters per row to start
	int array_length = 10; // need to keep track of this manually for heap-allocated memory
	char **buffer = init_2D_buffer(array_length, LINE_SIZE);
	buffer[4] = malloc(LINE_SIZE * sizeof(char));
	printf("here 1\n");
	array_length = read_into_buffer(file, buffer, array_length);
	printf("here 2\n");

	printf("buffer:\n");
	showall(buffer, array_length);

	printf("end of buffer\n");

	return 0;

	printf("hello from the main femto\n");
	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	printw("Ctrl+Q - Exit\n");
	refresh();
	int c;
	int x = 50;
  int y = 5;
	int height = 10;
  int width = 100;
  int starty = (LINES - height) / 2;
  int startx = (COLS - width) / 2;
	WINDOW *my_win;
	my_win = newwin(25, 50, y, x);
  wmove(my_win, y, x);
	while (1) {
			c = getch();
			if (c == 17){
				quit();
				break;
			}
			else{
				if (c == KEY_LEFT){
					 x--;
				 }
				if (c == KEY_RIGHT){
					 x++;
				 }
			  if (c == KEY_UP){
				  y--;
			  }
			  if (c == KEY_DOWN){
				  y++;
			  }
			  wmove(my_win, y, x);
			  wrefresh(my_win);
	 	}
	}
  return 0;
}
