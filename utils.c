#include <ncurses.h>
#include <ctype.h>
#include "utils.h"

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

