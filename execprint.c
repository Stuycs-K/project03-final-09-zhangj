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
#include <ncurses.h>
#include <string.h>
#include "execprint.h"
#include "filebuffer.h"

void parse_args( char * line, char ** arg_ary ){
	int i = 0;
	while (line) {
		char *token = strsep(&line, " ");
		arg_ary[i] = token;
		i++;
	}
	arg_ary[i] = NULL;
}

void redirect_stdout(int fd) {
	fflush(stdout); // unsure if needed
	
	dup2(fd, STDOUT_FILENO);
}

void do_exec(struct file_buffer *file_buff, int *pipe_fds, char **arg_array, char *error_message, int *x, int *y, int *height, int *width, int *curY, int *yLineEnd, int *top, int *bottom, int *has_error, int *changed) {
	if (pipe(pipe_fds) == -1) {
		fprintf(stderr, "Failed to pipe\n");
		endwin();
		exit(1);
	}
	
	int forkpid = fork();
	if (forkpid == -1) {
		fprintf(stderr, "Failed to fork");
		endwin();
		exit(1);
	} else if (forkpid == 0) { // child process
		close(pipe_fds[READ_FD]);
		redirect_stdout(pipe_fds[WRITE_FD]);

		int forkpid2 = fork();
		if (forkpid2 == -1) {
			fprintf(stderr, "Failed to fork 2");
			endwin();
			exit(1);
		} else if (forkpid2 == 0) {
			if (execvp(arg_array[0], arg_array) == -1) {
				exit(1);
			}
			
		} else {
			int status;
			waitpid(forkpid2, &status, 0);
			close(pipe_fds[WRITE_FD]);

			if (WEXITSTATUS(status) != 0) {
				exit(1);
			} else {
				exit(0);
			}
		}

	} else { // main process
		close(pipe_fds[WRITE_FD]);

		int status;
		waitpid(forkpid, &status, 0);
		// if child failed, do not attempt to read from the pipe
		if (WEXITSTATUS(status) != 0) {
			*has_error = 1;
			sprintf(error_message, "Error: Unable to execvp '%s' properly", arg_array[0]);
			close(pipe_fds[READ_FD]);
			return;
		}

		// copy everything from the child's command into the file_buffer
		int bytes_read;
		char line[LINE_SIZE];
		while ((bytes_read = read(pipe_fds[READ_FD], line, LINE_SIZE-1))) {
			line[bytes_read] = '\0';
			if (bytes_read == -1) {
				fprintf(stderr, "Failed to read from pipe\n");
				endwin();
				exit(1);
			}
			
			insert_at_end(file_buff, line);
			
		}

		if (bytes_read >= 1 && line[bytes_read-1] == '\n') {
			insert_row(file_buff, file_buff->rows);
		}

		close(pipe_fds[READ_FD]);
		
		(file_buff->rows)--;

		*y = file_buff->rows;
		*x = strlen(file_buff->buffer[*y-1])-1;
		*curY = *y;
		*yLineEnd = *y;

		while (*y >= *bottom){
			*bottom += *height-3;
			*top += *height-3;
			*curY -= *height-3;
		}
	}

	*changed = 1;

}
