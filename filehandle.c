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
#include "filehandle.h"

void error(char *message) {
	char error_string[256];
  sprintf(error_string, "%s: errno %d", message, errno);
  perror(error_string);
  exit(1);
}

int myopen(char *filename, int permissions) {
	int file_descriptor = open(filename, permissions, 0644);
	if (file_descriptor == -1) {
		error("Open file");
	}
	
	return file_descriptor;
}