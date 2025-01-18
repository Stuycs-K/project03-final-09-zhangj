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
#include "execprint.h"

void parse_args( char * line, char ** arg_ary ){
  int counter = 0;
  while (line){
    char * token;
    token = strsep(&line, " ");
    arg_ary[counter] = token;
    counter++;
  }
  arg_ary[counter] = NULL;
}