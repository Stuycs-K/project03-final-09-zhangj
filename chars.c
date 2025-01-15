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
#include "chars.h"
#include "filehandle.h"

int addChar(int x, int* y, int* yLineEnd, int* xLineEnd, int taboffset, int width, struct file_buffer *file_buff, char* c){
    if (x+taboffset>=width-1){
        insert_row(file_buff,*y);
        *y++;
        *yLineEnd++;
        x = 0;
        *xLineEnd = 0;
        insert_char(file_buff,*y,x,c);
        x++;
    }
    else{
        insert_char(file_buff,*y-1,x,c);
        x++;
    }
    return x;
}