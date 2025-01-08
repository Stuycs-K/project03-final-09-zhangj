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
#include <ncurses.h>

int main() {
    initscr();
    cbreak();        
    noecho();          
    keypad(stdscr, TRUE); 
    printf("Ctrl+Q - Exit\tCtrl+S - Save\n");
    int c;
    while (1) {
        c = getch(); 
        if (c == 19) { // Ctrl+S
            printf("Would you like to save your modified changes? y/n");
            char buff[256];
            buff[1] = '\0';
            fgets(buff, 255, stdin);
            if (buff[0] == 'y' && buff[1] == '\0'){
                printf("File name: ");
                char buff1[256];
                fgets(buff1, 255, stdin);
                printf("File name is: %s\n", buff1);
            }
            else{
                printf("Exiting...\n");
                break;
            } 
        }
        if (c == 17){ // Ctrl+Q
            printf("Exiting...\n");
            break;
        }
    }
    endwin();
    return 0;
}