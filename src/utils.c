#include <ncurses.h>

#include "../lib/utils.h"


char* char_input(WINDOW* window, char* output_stream, const int input_size, const char hidden_char){
    int ch, i = 0, y, x;

    while ((ch = wgetch(window)) != '\n') {
        if (ch != 127 && ch != 8) {
            if(i < input_size){
                wattron(window, COLOR_PAIR(5));
                output_stream[i] = ch;
            }else
                wattron(window, COLOR_PAIR(3));
            i++;
            if(hidden_char != 0)
                waddch(window, hidden_char);
            else
                waddch(window, ch);
        } else if ((ch == 127 || ch == 8) && i > 0) {
            i--;
            getyx(window, y, x);
            mvwaddch(window, y, x - 1, ' ');
            wmove(window, y, x - 1);
        }
        wrefresh(window);
    }

    output_stream[i] = '\0';
}