#ifndef FILEHANDLE_H
#define FILEHANDLE_H

FILE* open_read(char *filename);
void close_file(FILE *file);
void initFile(int argc, char *argv[], char* filename, char* fileinfo, FILE* file);

#endif