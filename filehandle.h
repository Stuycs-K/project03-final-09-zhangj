#ifndef FILEHANDLE_H
#define FILEHANDLE_H
#include <stdio.h>

FILE* myopen(char *filename);
void myclose(FILE *file);

#endif
