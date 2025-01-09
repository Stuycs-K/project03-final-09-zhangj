#ifndef FILEHANDLE_H
#define FILEHANDLE_H
#include <stdio.h>

#define LINE_SIZE 1000

FILE* myopen(char *filename);
void myclose(FILE *file);

void read_into_buffer(FILE *file, char **buffer);
void showall(char **buffer, int rows);

char** init_2D_buffer(int rows, int cols);
char** resize_2D_buffer(char **buffer, int rows);
char* resize_1D_buffer(char *buffer, int cols);

#endif
