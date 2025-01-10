# Dev Log:

This document must be updated daily by EACH group member.

## Julien Zhang

### 2025-01-06 - Updating documentation, creating code files
Updated proposal and readme with all necessary info. Done during class time (40 min). Created skeleton files for project including function headers. Done for HW (30 min).


### 2025-01-07 - Save and quit from the text editor
Looked into ncurses documentation and figured out how to intercept keyboard input. Set up basic function to ask the user if they want to save their changes (quit if not), then ask the user for the file name before quitting if Ctrl+Q is pressed. (40 min)

### 2025-01-08 - Cursor movement
Looked more into ncurses documenation and figured out how to move the cursor using arrow keys. Also started working on actually being able to type into the text editor. Planning to move these functions into a separate file tomorrow.

### 2025-01-09 - Deleting and framework for saving characters to a buffer
Implemented deleting from the text editor. Rewrote the way characters are printed to prepare for integrating into writing each character into a buffer. Worked a little on enter.

## David Schwartzberg

### 2025-01-06 - (wasn't part of group yet)
N/A

### 2025-01-07 - Makefile and file handlings
Reorganized to make a makefile with run, compile, test targets. Worked on abstracting out opening/closing files with wrapper functions.

### 2025-01-08 - Reading files into a buffer
Created new functions that can read the current file into a buffer and display it

### 2025-01-09 - Bug-fixing and insertion functions
Spent too long on finding a bug and created insertion functions for the file buffer
