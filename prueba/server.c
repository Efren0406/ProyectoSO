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

void* serverThread(void* arg) {
    int shmid;
    char* shared_memory;
    
    // Crear la memoria compartida
    shmid = shmget(SHARED_MEMORY_KEY, MAX_MESSAGE_LENGTH, IPC_CREAT | 0666);
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
        // Esperar a que el cliente escriba un mensaje
        sem_wait(semaphore_read);
        
        // Leer el mensaje de la memoria compartida y mostrarlo
        printf("Mensaje recibido: %s", shared_memory);
        
        // Terminar el servidor si el mensaje es "exit"
        if (strcmp(shared_memory, "exit\n") == 0) {
            break;
        }
        
        // Publicar el semáforo de escritura
        // sem_post(&semaphore_write);
    }
    
    // Desvincular y eliminar la memoria compartida
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);
    
    return NULL;
}

int main() {
    pthread_t tid;

    // Inicializar los semáforos
    semaphore_write = sem_open("papoi", O_CREAT, 0600, 0);
    semaphore_read = sem_open("papoi2", O_CREAT, 0600, 0);
    
    // Crear el hilo del servidor
    if (pthread_create(&tid, NULL, serverThread, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }
    
    // Esperar a que el hilo del servidor termine
    if (pthread_join(tid, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }
    
    // Destruir los semáforos
    sem_destroy(semaphore_write);
    sem_destroy(semaphore_read);
    
    return 0;
}