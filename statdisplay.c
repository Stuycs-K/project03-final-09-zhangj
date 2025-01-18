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
#include <math.h>
#include "filebuffer.h"

// system-specific things
#define LINUX 0
#define MACOS 1
#define UNKNOWN 2
#ifdef __linux__
    #define PLATFORM LINUX
#elif defined(__APPLE__) && defined(__MACH__)
    #define PLATFORM MACOS
#else
    #define PLATFORM UNKNOWN
#endif

// returns a string ending one of the below prefixes
// e.g. 0B, 999B, 1.0K, 35K, 100M
char suffixes[] = {'B', 'K', 'M', 'G', 'T', 'E', 'Z'};
char* size_to_suffix_string(unsigned long n) {
  double dn = (double) n;

  int suffix_i = 0;
  while (dn >= ((PLATFORM == MACOS) ? 1000 : 1024)) {
    dn /= 1024;
    suffix_i++;
  }

  char* prefix_string = (char*) malloc(5 * sizeof(char));
  if (suffix_i == 0) {
		sprintf(prefix_string, "%lu", n);
  } else {
    if (dn < 1) {
      sprintf(prefix_string, "1.0%c", suffixes[suffix_i]);
    } else if (dn < 10) {
      dn = (PLATFORM == MACOS) ? (round(dn * 10) / 10.0) : (ceil(dn * 10) / 10.0);
      if (dn < 10) {
        sprintf(prefix_string, "%1.1f%c", dn, suffixes[suffix_i]);
      } else {
        sprintf(prefix_string, "%2.0f%c", dn, suffixes[suffix_i]);
      }
    } else if (dn < 100) {
      dn = (PLATFORM == MACOS) ? (round(dn)) : (ceil(dn));
      sprintf(prefix_string, "%2.0f%c", dn, suffixes[suffix_i]);
    } else {
      dn = (PLATFORM == MACOS) ? (round(dn * 1000) / 1000.0) : (ceil(dn));
      sprintf(prefix_string, "%3.0f%c", dn, suffixes[suffix_i]);
    }
  }

  return prefix_string;
}

// returns a char* string with newlines to be displayed in the window
char* stat_info(char *path, char *stat_info_string) {
	struct stat *stat_buffer = malloc(sizeof(struct stat));
	stat(path, stat_buffer);

	snprintf(stat_info_string, LINE_SIZE-1, "%s: %s bytes; last modified %s", path, size_to_suffix_string(stat_buffer->st_size), ctime(&(stat_buffer->st_mtime)));
	stat_info_string[LINE_SIZE-1] = '\0';

	free(stat_buffer);
	return stat_info_string;
}
