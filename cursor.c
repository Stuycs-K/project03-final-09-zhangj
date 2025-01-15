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

void keyleft(int *x){
    if (*x > 0){
        *x--;
	}
}

// void keyright();

// void keyup();

// void keydown();