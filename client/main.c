#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>

#include "../lib/menus.h"
#include "../lib/server.h"

int main() {    
    int state = 0;
    sem_t *SEM = sem_open(SERVER_SEMAPHORE_NAME, O_CREAT, 0600, 0);

    sem_post(SEM);

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
