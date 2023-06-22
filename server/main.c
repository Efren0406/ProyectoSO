#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>

#include "../lib/server.h"

int shutDownFlag = 0;

void interruptionHandler(int sig){
    char c;

    signal(sig, SIG_IGN);

    printf("\nServidor en Pausa\n");
    printf("Todas los usuarios perderan conexión!!!\n");
    printf("Esta seguro de apagar el servidor? [y / n]\n");

    c = getchar();

    if(c == 'y' || c == 'Y')
        exit(1);
    else{
        system("clear");
        printf("\nServidor reanudado!\n");
        printf("Esperando usuarios...\n\n");

        signal(SIGINT, interruptionHandler);
    }

    getchar();
}

void main(){
    signal(SIGINT, interruptionHandler);

    system("clear");
    printf("Iniciando Servidor...\n");

    int newUserSHMID, fileSHMID, filenameSHMID;
    int *NEW_USER;
    sem_t *SEM;    

    newUserSHMID = shmget(NEW_USER_FLAG, sizeof(int), IPC_CREAT | 0666);
    fileSHMID = shmget(FILE_MEMORY_KEY, sizeof(FILE*), IPC_CREAT | 0666);
    filenameSHMID = shmget(FILENAME_MEMORY_KEY, sizeof(char), IPC_CREAT | 0666);

    NEW_USER = (int*)shmat(newUserSHMID, NULL, 0);

    *NEW_USER = 0;

    printf("Servidor Listo!...\n");
    printf("Esperando Usuarios...\n");

    do{
        if(*NEW_USER){
            printf("\nNuevo usuario contectado!\n");
            printf("Acceso otorgado\n");        
            *NEW_USER = 0;
        }
    } while(1);

    sem_destroy(SEM);
    
    shmctl(fileSHMID, IPC_RMID, NULL);
    shmctl(filenameSHMID, IPC_RMID, NULL);

    printf("\nServidor Apagado!!!\n");
    printf("Hasta luego.\n");
}