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
            endwin();
            printf("Would you like to save your modified changes? y/n ");
            char buff[256];
            buff[1] = '\0';
            fgets(buff, 255, stdin);
            printf("Buff: %s\n", buff);
            if (buff[0] == 'y' && buff[1] == '\0'){
                printf("File name: ");
                char buff1[256];
                fgets(buff1, 255, stdin);
                printf("File name is: %s\n", buff1);
                break;
            }
            else{
                break;
            } 
        }
        if (c == 17){ // Ctrl+Q
            endwin();
            break;
        }
    }
    
    return 0;
}