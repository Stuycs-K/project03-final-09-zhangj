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
#include <string.h>
#include "execprint.h"
#include "filebuffer.h"

void parse_args( char * line, char ** arg_ary ){
	int i = 0;
	while (line) {
		char *token = strsep(&line, " ");
		arg_ary[i] = token;
		i++;
	}
	arg_ary[i] = NULL;
}

void redirect_stdout(int fd) {
	fflush(stdout); // unsure if needed
	
	dup2(fd, STDOUT_FILENO);
}
