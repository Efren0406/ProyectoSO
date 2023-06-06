#include <ncurses.h>
#include <termios.h>
#include <unistd.h>

#include "../lib/menus.h"
#include "../lib/const.h"

int main() {    
    WINDOW *window;
    int state = 0;

    initscr();
    start_color();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    if(has_colors()){
        init_pair(1, COLOR_YELLOW, COLOR_BLACK);
        init_pair(2, COLOR_BLACK, COLOR_WHITE);
        init_pair(3, COLOR_RED, COLOR_BLACK);
        init_pair(4, COLOR_GREEN, COLOR_BLACK);
        init_pair(5, COLOR_WHITE, COLOR_BLACK);
    }

    wbkgd(stdscr, COLOR_PAIR(2));

    window = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 5 / 2, 5 / 2);
    wbkgd(window, COLOR_PAIR(5));

    refresh();

    while(1){
        if(state == -1){
            state = admin_menu();
        }else if(state == 0)
            state = login(window);
        else if(state == 1)
            state = menu();
        else if(state == 2)
            state = cart();
        else if(state == 3)
            state = purchase();
    }

    return 0;
}
