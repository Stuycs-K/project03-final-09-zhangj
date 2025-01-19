#ifndef FILEHANDLE_H
#define FILEHANDLE_H
#include <stdio.h>

FILE* open_read(char *filename);
void close_file(FILE *file);
FILE* initFile(int argc, char *argv[], char* filename, char* fileinfo);

#endif