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

int numDigits(int n){
  int count = 0;
  while (n != 0){
    n /= 10;
    count++;
  } 
  return count;
}

int keyleft(int x, int y){
    if (x > numDigits(y)+2){
        x--;
	}
    return x;
}

int keyright(int x, int xLineEnd){
    if (x < xLineEnd){
        x++;
    }
    return x;
}

int keyup(int* x, int y, int lineLen, int* curY){
    if (y > 1){
        y--;
        *curY-=1;
        if (*x > lineLen){
            *x = lineLen;
        }
    }
    return y;
}

int keydown(int* x, int y, int yLineEnd, int numrows, int linelen, int* curY){
    if (y < yLineEnd){
        y++;
        *curY-=1;
        if (y == numrows){
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

