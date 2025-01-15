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

int keyleft(int x){
    if (x > 0){
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

int keyup(int* x, int y, struct file_buffer *file_buff){
    if (y > 1){
        y--;
        if (*x > strlen(file_buff->buffer[y-1])-1){
            *x = strlen(file_buff->buffer[y-1])-1;
        }
    }
    return y;
}

// void keydown();