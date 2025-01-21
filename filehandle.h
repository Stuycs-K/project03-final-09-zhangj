#ifndef FILEHANDLE_H
#define FILEHANDLE_H
#include <stdio.h>

#define UNTITLED_FILENAME "Untitled.txt"

FILE* open_read(char *filename);
void close_file(FILE *file);
FILE* init_file(int argc, char **argv, char* filename, char* fileinfo);

#endif
