#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

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
        shutDownFlag = 1;
    else
        signal(SIGINT, interruptionHandler);

    getchar();
}

void main(){
    signal(SIGINT, interruptionHandler);

    system("clear");
    printf("Iniciando Servidor...\n");

    pthread_attr_t atributes;
    pthread_t ID[MAX_USER_COUNT];
    int fileSHMID, filenameSHMID;
    sem_t *SEM;    

    pthread_attr_init(&atributes);
    pthread_attr_setdetachstate(&atributes, PTHREAD_CREATE_DETACHED);    

    fileSHMID = shmget(FILE_MEMORY_KEY, sizeof(FILE), IPC_CREAT | 0666);
    filenameSHMID = shmget(FILENAME_MEMORY_KEY, sizeof(char), IPC_CREAT | 0666);

    SEM = sem_open(SERVER_SEMAPHORE_NAME, O_CREAT, 0600, 0);

    printf("Servidor Listo!...\n");
    printf("Esperando Usuarios...\n");

    do{
        sem_wait(SEM);

        printf("Nuevo usuario contectado!\n");
        printf("Acceso otorgado\n");        

        pthread_create(&ID[i], &atributes, (void*)userListener, NULL);        
    } while(!shutDownFlag);

    sem_destroy(SEM);
    
    shmctl(fileSHMID, IPC_RMID, NULL);
    shmctl(filenameSHMID, IPC_RMID, NULL);

    printf("\nServidor Apagado!!!\n");
    printf("Hasta luego.\n");
}