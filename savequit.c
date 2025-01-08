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
#include <curses.h>

int main() {
    initscr();
    raw();        
    noecho();          
    keypad(stdscr, TRUE); 
    printw("Ctrl+Q - Exit\tCtrl+S - Save\n");
    refresh();
    int c;
    while (1) {
        c = getch(); 
        if (c == 19) { // Ctrl+S
            printw("Would you like to save your modified changes? y/n");
            refresh();
            char buff[256];
            buff[1] = '\0';
            fgets(buff, 255, stdin);
            if (buff[0] == 'y' && buff[1] == '\0'){
                printw("File name: ");
                refresh();
                char buff1[256];
                fgets(buff1, 255, stdin);
                printw("File name is: %s\n", buff1);
                refresh();
                break;
            }
            else{
                printw("Exiting...\n");
                refresh();
                break;
            } 
        }
        if (c == 17){ // Ctrl+Q
            printw("Exiting...\n");
            refresh();
            break;
        }
    }
    endwin();
    return 0;
}