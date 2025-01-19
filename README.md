# Femto (Terminal Text Editor)

### (really cool and awesome team name here)
#### Group Id: 20
- Julien Zhang
- David Schwartzberg
- Group Id: 20

### Project Description:

Our project is a simple text editor that runs in the terminal. It allows the user to create and edit text files

### Requirements:
ncurses installed. 

### Instructions
to run the program, either: `make run`, or first `make compile` and then `./femto`. There are optional command line arguments. Use them as follows: <br>
`make run ARGS="<filename here>"` or <br>
`./femto "<filename here>"`

If the program is run with no args (`./femto` or `make run`), the program will create an untitled file and only prompt for the filename upon the first save. If the program is run with args:
- if the file exists, the contents of that file will be shown to the user.
- otherwise, the file will be created and saved to with that filename on save.
  
#### Keyboard Shortcuts
(these are also displayed on the top of the screen at all times)

- Ctrl+Q: Exit <br>
Exits the program. If the user has not saved since their last change, they will be prompted to save before exiting. If the filename is "untitled.txt", they will be asked to input a filename.

- Ctrl+S: Save <br>
Saves the current buffer to a file. If the filename is still "untitled.txt", they will be asked to input a filename. On future saves, the program will automatically save to that filename.

- Ctrl+T: Execute <br>
Allows to user to enter a single simple terminal command and outputs that into the current file buffer. (say stuff here about what commands are valid, <, >, | are all bad)

- Ctrl+G - Go to line # <br>
Allows the user to go to a line. If the line number is not valid, does nothing.