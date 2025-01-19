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
these are also displayed on the top of the scren at all times

The user runs make bano with one arg which is the file name. If that file exists the text from that file is displayed, if not the program will create it. If no arg is given the file will be untitled for now until it is given a name later. In the text editor the user can type into stdin and use different terminal signals such as ctrl+c to perform special actions. The user can use ctrl+x to exit the text editor and a prompt is displayed asking if the user wants their changes to be saved. If yes, the text that was written is written to a file and the user is asked for the name of the file to save the text to. If not the text editor exits and no changes are made.
