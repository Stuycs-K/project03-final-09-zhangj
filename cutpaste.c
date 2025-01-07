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

// Takes in the current buffer, where the desired cut should be made, how much should be cut, edits the buffer accordingly
char* cut(char* buffer, int location, int length){

}

// Takes in the whatever was previously cut from the text editor and edits the buffer by pasting it in the desired location
char* paste(char* buffer, int location){

}