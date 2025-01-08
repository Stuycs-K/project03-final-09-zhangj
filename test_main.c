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
#include "undoredo.h"
#include "cutpaste.h"
#include "writesave.h"
#include "statdisplay.h"
#include "filehandle.h"

// main file for testing in case the current main is fragile
int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("argv[1] must indicate the filename\n");
		exit(1);
	}
	
	int fd = myopen(argv[1]);
	printf("fd: %d\n", fd);
  return 0;
}