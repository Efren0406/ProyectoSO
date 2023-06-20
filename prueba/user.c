#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define SHARED_MEMORY_KEY 1234
#define SHARED_SEMAPHORE_KEY 12345
#define MAX_MESSAGE_LENGTH 100

sem_t *semaphore_write, *semaphore_read;

void* clientThread(void* arg) {
    int shmid, semid;
    char* shared_memory;
    
    // Crear o acceder a la memoria compartida existente
    shmid = shmget(SHARED_MEMORY_KEY, MAX_MESSAGE_LENGTH, 0666);
    semid = shmget(SHARED_SEMAPHORE_KEY, sizeof(int), 0666);

    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
    
    // Adjuntar la memoria compartida
    shared_memory = (char*) shmat(shmid, NULL, 0);

    if (shared_memory == (char*) -1) {
        perror("shmat");
        exit(1);
    }
    
    while (1) {
        // sem_wait(&semaphore_write);

        printf("Ingrese un mensaje (o 'exit' para salir): ");
        fgets(shared_memory, MAX_MESSAGE_LENGTH, stdin);
        
        sem_post(semaphore_read);

        // Salir si el mensaje es "exit"
        if (strcmp(shared_memory, "exit\n") == 0) {
            break;
        }
    }
    
    // Desvincular la memoria compartida
    shmdt(shared_memory);
    
    return NULL;
}

int main() {
    pthread_t tid;
    
    // Inicializar los semáforos
    // sem_init(&semaphore_write, 0, 0);
    // sem_init(&semaphore_read, 0, 0);
    semaphore_write = sem_open("papoi", O_CREAT, 0600, 0);
    semaphore_read = sem_open("papoi2", O_CREAT, 0600, 0);
    
    // Crear el hilo del cliente
    if (pthread_create(&tid, NULL, clientThread, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }
    
    if(pthread_join(tid, NULL) != 0){
        perror("pthread_join");
        exit(1);
    }
    
    // Destruir los semáforos
    // sem_destroy(&semaphore_write);
    // sem_destroy(&semaphore_read);
    
    return 0;
}