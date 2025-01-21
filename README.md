# Femto (Terminal Text Editor)

## Group Name: Fearless Femto Fellows
## Group Id: 20
## Group Members:
- Julien Zhang
- David Schwartzberg

## Project Description:

Our project is a simple text editor that runs in the terminal, inspired by the Nano text editor. It allows the user to create, edit, and save text files.

## Requirements:
- Ncurses library (Installation tutorial: https://www.cyberciti.biz/faq/linux-install-ncurses-library-headers-on-debian-ubuntu-centos-fedora/)

## Video Link: https://drive.google.com/file/d/17yKLH9yjN57_Ud1H3o0JkDfSO8UjjuG7/view?usp=drive_link

## Instructions
### How to Run the Program
To run the program, either: `make run`, or first `make compile` and then `./femto`. There is one optional command line argument. It is utilized as follows: <br>
- `make run ARGS="<file name>"` or <br>
- `./femto "<file name>"`

If the program is run with no args (`./femto` or `make run`), the program will create a temporary file titled "Untitled.txt" that the user can write in to and only prompt for the filename upon the first save or quit. <br>

If the program is run with a filename argument: <br>
- If the file exists, the contents of that file will be read and shown to the user. 
- Otherwise, the file will be created and saved to with that filename on save or quit. <br>

If the program is run with more than one argument it'll error out and display a message repeating these directions.
  
### Keyboard Shortcuts
(These are also displayed on the top of the screen at all times while running the program)

- Ctrl+Q: Exit <br>
Exits the program. If the user has not saved since their last change, they will be prompted to save before exiting. If the file name is "Untitled.txt", they will be asked to input a file name. If you are overwriting a file you will be asked to confirm the overwrite.

- Ctrl+S: Save <br>
Saves the current buffer to a file. If the file name is "Untitled.txt", they will be asked to input a file name.  If you are overwriting a file you will be asked to confirm the overwrite. On future saves, the program will automatically save to that file name.

- Ctrl+T: Execute <br>
Allows to user to enter a single terminal command and appends the output to the end of the current file buffer. Only supports VERY basic commands with VERY basic text outputs (no piping, redirecton, animated output like `sl`, etc.). Otherwise, prints an error and does nothing.

- Ctrl+G - Go to line # <br>
Allows the user to enter a line number and go to that line. If the line number is not valid, prints an error and does nothing.

### Typing into the text buffer
All single character ASCII text is supported. Anything you type will be enter into the file buffer and displayed on the terminal window. Here is the list of special keyboard input that is supported:

- All keyboard shortcuts listed above

- Enter: <br>
Creates a new line at the current cursor position.

- Backspace: <br>
Deletes a character (including new lines if applicable) at the current cursor position.

- Tab: <br>
Inputs a tab at the current cursor position. This works the same as it does in Nano so the tab length is not always consistent. Has a lmitation where you can't use tab on the second display line of a long line (more details below).

- Arrow Keys: <br>
Moves the cursor one space in the direction of the arrow key that is pressed. Is bounded by the text that is currently displayed on screen, as well as the informational text on the top and bottom of the window and the size of the window.

### Long Lines and Scrolling
- Long Lines: <br>
If you type past the width of the terminal window any overflow text will be displayed on the next line. This is purely visual and when you save the file all the text of that long line will be displayed in one line in the text file. Has limitations with tabs and is capped out at a max of two display lines (2x the window width).

- Scrolling: <br>
If you type past the length of the terminal window the terminal window will scroll to the next section of the file buffer. Only sections of the file buffer relative to the size of the terminal window are displayed on screen at one time. You may use the mouse scroll wheel, the up and down arrow keys, or the "Go to line" keyboard shortcut to move between sections of the file buffer.

## Other Known Limitations
- You can’t resize the terminal window while the program is running
- File name must contain only alphanumeric characters and/or periods (.)
- File must be in the same directory as the program
- Maximum number of lines is 999
- Line size must not go over 1000 (technically should not be possible due to line length limitation of 2x the window length)
- Only named keyboard shortcuts are supported (no copy, cut, paste, etc.)
- File contents must be ASCII text (no raw byte data)
- Filename cannot be “Untitled.txt”
- Keyboard shortcuts (such as Ctrl+Q) are not supported while you are in the input prompt of another keyboard shortcut (such as Ctrl+G)
- You can’t move the cursor position in the input prompt of a keyboard shortcut

## "Almost Working"/Likely Slightly Buggy Features
- Execute (Ctrl+T)
- Long lines
- Window Scrolling
- Tabs
- Make run with very large/long files

