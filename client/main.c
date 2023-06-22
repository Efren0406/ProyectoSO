#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/shm.h>

#include "../lib/menus.h"
#include "../lib/server.h"

int main() {    
    int state = 0, newUserSHMID;
    int *newUserFlag;

    newUserSHMID = shmget(NEW_USER_FLAG, sizeof(int), IPC_CREAT | 0666);
    newUserFlag = (int*)shmat(newUserSHMID, NULL, 0);

    *newUserFlag = 1;

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
