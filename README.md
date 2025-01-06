[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Vh67aNdh)
# Text Editor (Nano)

### Bano (Bad Nano)

- Julien Zhang
       
### Project Description:

Creating a text editor that is similar to nano and can be run through the terminal. Will include basic features of nano such as creating and saving files, editing files, and simple qol features such as redo/undo, cut/paste. May also include other features such as search, go to line, replace, etc.
Intended usage:
  
### Instructions:

The user runs make with one arg which is the file name. If that file exists the text from that file is displayed, if not the program will create it. If no arg is given the file will be untitled for now until it is given a name later. In the text editor the user can type into stdin and use different terminal signals such as ctrl+c to perform special actions. The user can use ctrl+x to exit the text editor and a prompt is displayed asking if the user wants their changes to be saved. If yes, the text that was written is written to a file and the user is asked for the name of the file to save the text to. If not the text editor exits and no changes are made.
