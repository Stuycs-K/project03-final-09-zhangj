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
