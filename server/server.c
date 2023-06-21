#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "../lib/const.h"
#include "../lib/server.h"

void userListener(void) {
  sem_t *SEM;
  int fileSHMID, filenameSHMID;
  FILE *database;
  char *filename;

  SEM = sem_open(CLIENT_SEMAPHORE_NAME, O_CREAT, 0600, 0);

  fileSHMID = shmget(FILE_MEMORY_KEY, sizeof(FILE), IPC_CREAT | 0666);
  filenameSHMID = shmget(FILE_MEMORY_KEY, sizeof(char), IPC_CREAT | 0666);

  database = (FILE *)shmat(fileSHMID, NULL, 0);
  filename = (char *)shmat(filenameSHMID, NULL, 0);

  do {
    sem_wait(SEM);

    database = fopen(filename, "r");

    printf("El usuario a accedido a la base de datos %s", filename);
  } while (1);

  shmdt(filename);
  shmdt(database);
  shmctl(fileSHMID, IPC_RMID, NULL);
  shmctl(filenameSHMID, IPC_RMID, NULL);
}