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
    sem_t *SEM;

    int i = 0;

    int *userNum;

    pthread_attr_init(&atributes);
    pthread_attr_setdetachstate(&atributes, PTHREAD_CREATE_DETACHED);

    SEM = sem_open(SERVER_SEMAPHORE_NAME, O_CREAT, 0600, 0);

    printf("Servidor Listo!...\n");
    printf("Esperando Usuarios...\n");

    do{
        sem_wait(SEM);

        printf("Nuevo usuario contectado!\n");
        printf("Acceso otorgado\n");

        userNum = i;

        pthread_create(&ID[i], &atributes, (void*)userListener, (void*)userNum);

        i++;
    } while(1);

    sem_destroy(SEM);

    printf("\nServidor Apagado!!!\n");
    printf("Hasta luego.\n");
}