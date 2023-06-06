#include "../lib/menus.h"
#include "stdlib.h"

int main() {    
    int state = 0;

    while(1){
        if(state == -1){
            state = admin_menu();
            system("clear");
        }else if(state == 0){
            state = login();
            system("clear");
        }else if(state == 1){
            state = menu();
            system("clear");
        }else if(state == 2){            
            state = cart();
            system("clear");
        }
    }

    return 0;
}
