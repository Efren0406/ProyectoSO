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

void interruptionHandler(int sig){
    char c;

    signal(sig, SIG_IGN);

    printf("\n\nServidor en Pausa\n");
    printf("Todas los usuarios perderan conexión!!!\n");
    printf("Esta seguro de apagar el servidor? [y / n]\n");

    c = getchar();

    if(c == 'y' || c == 'Y')
        exit(1);
    else{
        int newUserSHMID, userCountSHMID, adminUpdateSHMID;
        int *NEW_USER, *USER_COUNT, *ADMIN_UPDATE;

        newUserSHMID = shmget(NEW_USER_FLAG, sizeof(int), IPC_CREAT | 0666);
        userCountSHMID = shmget(USER_COUNT_KEY, sizeof(int), IPC_CREAT | 0666);
        adminUpdateSHMID = shmget(ADMIN_UPDATE_KEY, sizeof(int), IPC_CREAT | 0666);

        shmctl(newUserSHMID, IPC_RMID, NULL);
        shmctl(userCountSHMID, IPC_RMID, NULL);
        shmctl(adminUpdateSHMID, IPC_RMID, NULL);

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

    int newUserSHMID, userCountSHMID, adminUpdateSHMID;
    int *NEW_USER, *USER_COUNT, *ADMIN_UPDATE;
    int firstFlag = 0;
    pthread_t threadid;

    newUserSHMID = shmget(NEW_USER_FLAG, sizeof(int), IPC_CREAT | 0666);
    userCountSHMID = shmget(USER_COUNT_KEY, sizeof(int), IPC_CREAT | 0666);
    adminUpdateSHMID = shmget(ADMIN_UPDATE_KEY, sizeof(int), IPC_CREAT | 0666);

    NEW_USER = (int*)shmat(newUserSHMID, NULL, 0);
    USER_COUNT = (int*)shmat(userCountSHMID, NULL, 0);
    ADMIN_UPDATE = (int*)shmat(adminUpdateSHMID, NULL, 0);

    *NEW_USER = 0;
    *USER_COUNT = 0;
    *ADMIN_UPDATE = 0;

    printf("Servidor Listo!...\n");
    printf("ID del servidor %d\n", getpid());
    printf("Esperando Usuarios...\n");

    do{
        if(*NEW_USER){
            firstFlag = 1;

            printf("\nNuevo usuario contectado!\n");
            printf("Acceso otorgado\n");        

            if(*USER_COUNT == 0)
                pthread_create(&threadid, NULL, (void*)userListener, NULL);

            *NEW_USER = 0;
            *USER_COUNT += 1;

            printf("\nNúmero de usuarios conectados: %d\n", *USER_COUNT);
        }

        if(*USER_COUNT == 0 && firstFlag){
            system("clear");
            printf("Todos los usuarios desconectados!\n");
            printf("Esperando nuevas conexiones...\n");

            firstFlag = 0;
        }

        if(*ADMIN_UPDATE == -1){
            printf("\nBase de datos actualizada por el administrador!\n\n");
            printf("Mostrando actualizaciones a usuarios...\n");

            *ADMIN_UPDATE = *USER_COUNT - 1;
        }
    } while(1);
}