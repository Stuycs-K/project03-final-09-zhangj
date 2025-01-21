# Final Project Proposal

## Group Members:
- Julien Zhang
- David Schwartzberg
       
## Intentions:

Creating a text editor that is similar to Nano and can be run through the terminal. Will include basic features of Nano such as creating files, saving files, and editing files. May include simple QOL features such as search, go to line, replace, copy/paste, redo/undo, etc. if time permits.
    
## Intended usage:

To run the program, either: `make run`, or first `make compile` and then `./femto`. There is one optional command line argument. It is utilized as follows: <br>
- `make run ARGS="<file name>"` or <br>
- `./femto "<file name>"`

If the program is run with no args (`./femto` or `make run`), the program will create a temporary file titled "Untitled.txt" that the user can write in to and only prompt for the filename upon the first save or quit. <br>

If the program is run with a filename argument: <br>
- If the file exists, the contents of that file will be read and shown to the user. 
- Otherwise, the file will be created and saved to with that filename on save or quit. <br>

If the program is run with more than one argument it'll error out and display a message repeating these directions.
  
## Technical Details:

### Topics covered in class that we will be using:
- **Allocating memory:** Using blocks of memory to store text such as the file data and user input
- **File operations:** Creating files, reading files, writing to files and saving them
- **Finding information about files:** Displaying file info in the program such as file name, file size, and date last modified
- **Signals:** Intercepting signals to ensure program exits gracefully (mainly just catching Segfaults since Ncurses handles the rest)
- **Processes:** Forking to execvp the command the user inputs from Ctrl+T (execute)
- **Pipes:** Creating a pipe to send the output of the execvp from the child to the parent
     
### How we are breaking down the project and who is responsible for which parts:
- **Julien:** All things Ncurses: catching keyboard input and writing it to the file buffer correctly, moving the cursor, displaying the file buffer into a Ncurses window
- **David:** All things not Ncurses: opening/creating/saving/stating files, creating a file buffer struct (2d char array) to store text data, forking, execvp, and piping associated with Ctrl+T (execute)
  
### Data structures/algorithms we will be using:
- **File buffer struct**: 2d char array that stores the number of rows, the length of each row, and each individual char array
- **Strings**: Store text data such as user input from keyboard shortcuts such as Ctrl+G (go to line)
    
## Intended pacing:
- **1/6:** Finish proposal and readme, create skeleton files
- **1/7-1/13:** Get basic functionality working, display file info in the text editor
- **1/14-1/19:** Start adding basic QOL features of Nano, such as go to line and execute
- **1/20-1/21:** Test for bugs and add other QOL features of Nano if time permits, make script and record video

