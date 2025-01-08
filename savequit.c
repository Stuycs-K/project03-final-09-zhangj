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
    printw("Ctrl+Q - Exit\n");
    refresh();
    int c;
    while (1) {
        c = getch(); 
        if (c == 17) { // Ctrl+Q
            endwin();
            printf("Would you like to save your modified changes? y/n ");
            char buff[256];
            buff[256] = '\0';
            fgets(buff, 255, stdin);
            if (buff[0] == 'y'){
                printf("File name: ");
                char buff1[256];
                fgets(buff1, 255, stdin);
                printf("File name is: %s\n", buff1);
                break;
            }
            else if (buff[0] == 'n'){
                printf("Quitting...\n");
                break;
            }
            else{
                printf("Invalid input. Quitting without saving...\n");
            } 
        }
    }
    
    return 0;
}