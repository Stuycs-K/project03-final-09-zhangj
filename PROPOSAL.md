# Final Project Proposal

## Group Members:
- Julien Zhang
- David Schwartzberg
       
## Intentions:

Creating a text editor that is similar to nano and can be run through the terminal. Will include basic features of nano such as creating and saving files, editing files, and simple qol features such as redo/undo, cut/paste. May also include other features such as search, go to line, replace, etc.
    
## Intended usage:

The user runs make bano with one arg which is the file name. If that file exists the text from that file is displayed, if not the program will create it. If no arg is given the file will be untitled for now until it is given a name later. In the text editor the user can type into stdin and use different terminal signals such as ctrl+c to perform special actions. The user can use ctrl+x to exit the text editor and a prompt is displayed asking if the user wants their changes to be saved. If yes, the text that was written is written to a file and the user is asked for the name of the file to save the text to. If not the text editor exits and no changes are made.
  
## Technical Details:

### Topics covered in class that I will be using:
- **Allocating memory:** Using blocks of memory to store data that user inputs
- **File operations:** Creating files, reading files, writing to files and saving them
- **Finding information about files:** Checking file permission before allowing editing of a file, displaying file info in the text editor such as file size
- **Signals:** Intercepting signals to perform special actions, such as undo/redo and cut/paste
     
### How I am breaking down the project and who is responsible for which parts:
- **Julien:** All things ncurses: catching keyboard input, moving the cursor, displaying text into an ncurses window
- **David:** All things not ncurses: opening/creating/saving/stating files, creating a file buffer struct (2d char array) to store text data
  
### Data structures/algorithms I will be using:
- **Strings**: Buffer to read text from file, storing text for redo/undo, buffer to write to file, etc.
    
# Intended pacing:
- **1/6:** Finish proposal and readme, create skeleton files
- **1/7-1/10:** Get basic functionality working, display file info in the text editor
- **1/13-1/17:** Start adding basic qol features of nano, implement redo/undo and cut/paste
- **1/18-1/21:** Test for bugs and add other features such as search, go to line, and replace if time permits

