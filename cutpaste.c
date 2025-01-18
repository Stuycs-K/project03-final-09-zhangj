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
#include "filebuffer.h"

// copies (and maybe removes) the string ranging from (r1, c1) to (r2, c2) into the copy-buffer
// "remove_string" is a boolean that will also modify the buffer to remove the string if it is non-zero
void cut_string(char *copy_buffer, struct file_buffer *file_buff, int r1, int c1, int r2, int c2, int remove_string) {
	
}

// pastes the copy-buffer at (r, c), making newlines if neccesary
void paste_string(char *copy_buffer, struct file_buffer *file_buff, int r, int c) {
	
}
