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

### 2025-01-13 - Fixed bugs and adding tab support
Worked on implementing make run without args to create a temp Untitled.txt file to write in (40 min CW time). Fixed bugs involving seg faults with files that did not end in a new line, running make run without any args, and text wrapping if you typed past the terminal window width. Also added tab support (1.5 hr HW time).

### 2025-01-14 - Fixed bugs, implemented save (Ctrl+s), moved cursor movement to separate file
Worked on implementing save (Ctrl+s) (40 min CW time). Fixed bugs with save, condensed main, created cursor.c to store cursor functions (1 hr HW time).

### 2025-01-15 - Started implementing typing past window height support
Worked on allowing typing past the window height. Tried using scroll features from ncurses but they broke everything. Settled on only displaying a certain section of the buffer instead (40 min Cw time + 1 hr HW time).

### 2025-01-16 - Implemented line numbers, typing past window length, started go to line
Added line numbers to the window. Added display for cursor location in the top left (40 min CW time). Implemented support for typing past window length, to be fully implemented once buffer code for line break is updated. Start the go to line command (Ctrl+G) (1h hr time).

### 2025-01-17 - Fixed bugs with cursor movement from typing past window length, implemented go to line, implemented input file name on first save for an untitled file, started work on execute
Fixed a few bugs with moving the cursor with arrow keys when you typed past the window length and went back up. Fully implemented go to line (Ctrl+G) (40 min CW). Implemented inputting the file name on first save if the file is untitled (make run with no args) and writing the buffer to that new file. Started framework on execute (Ctrl+T) (45 min HW time).

### 2025-01-18 - Fixed error handling, created new file filehandle.c, worked on fixing bugs involving saving/typing past window length, changed cursor position setup
Fixed error handling so that all errors exit the ncurses window when on it, moved file functions to new file filehandle.c and moved file initilization in main.c to filehandle.c, fixed bugs involving saving and typing past window length (while discovering more), changed cursor position setup to not use a unncessary loop




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

### 2025-01-11, 01-12 - Stat and organization
Integrated stat() info to be shown during display of the program, added extra comments to filehandle functions, moved around some other things for organization

### 2025-01-13 - Cleaning up bugs and extra information for stat
Fix bugs relating to reallocating memory, changed the way stat is displayed and add extra information to the string. Added new function headers for cutpaste.h file 

### 2025-01-14 - Saving issues and other
Fixed many issues for user saving on nonexistent files/no arguments. Added structure for copy/paste and execute commands.

### 2025-01-15 - Insert newlines and saving with untitled
Made function for inserting newline because it needs special case checking for many cases and tried to make user enter filename if they had not initially entered args.

### 2025-01-16 - Deleting newlines
Fixed further issues with insertion of newlines and made function for deleting newlines.

### 2025-01-17 - Bug-fixing and working more towards exec functionality
Made all newline functions work now, started to refactor the main loop to make the exec sections easier
