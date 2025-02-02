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
#include "utils.h"

// parses the arguments in line, split over spaces
void parse_args(char *line, char **arg_ary){
	int i = 0;
	while (line) {
		char *token = strsep(&line, " ");
		arg_ary[i] = token;
		i++;
	}
	arg_ary[i] = NULL;
}

// redirects stdout, only to be used by children of the main program
void redirect_stdout(int fd) {
	fflush(stdout);
	dup2(fd, STDOUT_FILENO);
}

// called by the main program itself
// first call prep_exec (below)
// appends output of the execvp'd program to the file buffer
// sets all cursor variables as needed
// sets show_message and message if input was bad
void do_exec(struct file_buffer *file_buff, char **arg_array, char *error_message, int *x, int *y, int *height, int *width, int *curY, int *yLineEnd, int *top, int *bottom, int *has_error, int *changed) {
	int pipe_fds[2];
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

			fprintf(stderr, "line read: '%s'", line);
			insert_at_end(file_buff, line);
			
		}

		insert_row(file_buff, file_buff->rows);
		if (bytes_read >= 1 && line[bytes_read-1] == '\n') {
			//insert_row(file_buff, file_buff->rows);
		} else {
			(file_buff->rows)--;
		}

		close(pipe_fds[READ_FD]);

		*y = file_buff->rows;
		*x = strlen(file_buff->buffer[*y-1]);
		*curY = *y;
		*yLineEnd = *y;

		while (*y >= *bottom){
			*bottom += *height-3;
			*top += *height-3;
			*curY -= *height-3;
		}

		(*curY) = (*y) % ((*height)-3);
	}

	*changed = 1;

}

// gets input from my_fgets, parses those args into the arg_array
void prep_exec(WINDOW *win, int height, int width, char *command_line, char **arg_array) {
	clear_fgets_line(win, height, width);
	mvwprintw(win, height-2, 0, "Enter Command: ");
	wrefresh(win);
	
	my_fgets(win, command_line, height, 32, 126, 15);
	int command_line_length = strlen(command_line);
	if (command_line[command_line_length-1] == '\n') { // strip newline
		command_line[command_line_length-1] = '\0';
	}
	parse_args(command_line, arg_array);
}