#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/shm.h>
#include <signal.h>

#include "../lib/menus.h"
#include "../lib/server.h"

void interruptionHandler(int sig){
    char c;
    int fileDirSHMID, filenameSHMID, newAccessSHMID;
    int *NEW_ACCESS;

    signal(sig, SIG_IGN);

    printf("\n\nEsta seguro de terminar la sesión? [y / n]\n");

    c = getchar();

    if(c == 'y' || c == 'Y'){
        newAccessSHMID = shmget(NEW_ACCESS_FLAG, sizeof(int), IPC_CREAT | 0666);
        NEW_ACCESS = (int*)shmat(newAccessSHMID, NULL, 0);

        *NEW_ACCESS = 2;

        shmdt(NEW_ACCESS);

        exit(1);
    }else
        signal(SIGINT, interruptionHandler);

    getchar();
}

int main() {    
    signal(SIGINT, interruptionHandler);

    int state = 0, newUserSHMID;
    int *NEW_USER;
    pthread_t pid;

    newUserSHMID = shmget(NEW_USER_FLAG, sizeof(int), IPC_CREAT | 0666);
    NEW_USER = (int*)shmat(newUserSHMID, NULL, 0);

    *NEW_USER = 1;

    printf("Esperando al servidor...\n");

    while(*NEW_USER);

    shmdt(NEW_USER);

    pthread_create(&pid, NULL, (void*)serverListener, NULL);

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
