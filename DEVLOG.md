# Dev Log:

This document must be updated daily by EACH group member.

## Julien Zhang

### 2025-01-06 - Updating documentation, creating code files
Updated proposal and readme with all necessary info (40 min class time). Created skeleton files for project including function headers (30 min HW time).

### 2025-01-07 - Save and quit from the text editor
Looked into ncurses documentation and figured out how to intercept keyboard input. Set up basic function to ask the user if they want to save their changes (quit if not), then ask the user for the file name before quitting if Ctrl+Q is pressed. (40 min class time + 30 min HW time).

### 2025-01-08 - Cursor movement
Looked more into ncurses documenation and figured out how to move the cursor using arrow keys. Also started working on actually being able to type into the text editor. Planning to move these functions into a separate file tomorrow (40 min class time + 30 min HW time).

### 2025-01-09 - Deleting and framework for saving characters to a buffer
Implemented deleting from the text editor. Rewrote the way characters are printed to prepare for integrating into writing each character into a buffer. Worked a little on enter (40 min class time + 30 min HW time).

### 2025-01-10 - Integrating the file buffer with display, fixing bugs with cursor movement
Integrated adding chars (including newline) into the file buffer with displaying them in the ncurses window, fixed bugs with cursor movement that resulted from this change (40 min class time).

### 2025-01-11 - Integrating delete with display, added cursor checks, integrated saving with quitting
Integrated deleting chars (including newline) into the file buffer with displaying them in the ncurses window, added checks so that you can't move the cursor outside of the text that has been typed and so that it auto moves the cursor to the end of a line if the previous line is larger than it, integrated saving with quitting after pressing Ctrl+Q (1.5 hr HW time).



## David Schwartzberg

### 2025-01-06 - (wasn't part of group yet)
N/A

### 2025-01-07 - Makefile and file handlings
Reorganized to make a makefile with run, compile, test targets. Worked on abstracting out opening/closing files with wrapper functions.

### 2025-01-08 - Reading files into a buffer
Created new functions that can read the current file into a buffer and display it

### 2025-01-09 - Bug-fixing and insertion functions
Spent too long on finding a bug and created insertion functions for the file buffer

### 2025-01-10 - Saving and deletion
Made functions that can save the buffer to a file and directly characters/lines from the buffer

### 2025-01-11, 01-12 - Stat and 
Integrated stat() info to be shown during display of the program, added extra comments to filehandle functions, moved around some other things for organization