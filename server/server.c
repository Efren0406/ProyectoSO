#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "../lib/const.h"
#include "../lib/server.h"

void userListener(void) {
  int fileDirSHMID, filenameSHMID, newAccessSHMID, userCountSHMID;
  int *NEW_ACCESS, *USER_COUNT;
  char *filename, *fileDir;

  newAccessSHMID = shmget(NEW_ACCESS_FLAG, sizeof(int), IPC_CREAT | 0666);
  userCountSHMID = shmget(USER_COUNT_KEY, sizeof(int), IPC_CREAT | 0666);
  filenameSHMID = shmget(FILENAME_MEMORY_KEY, sizeof(char), IPC_CREAT | 0666);
  fileDirSHMID = shmget(FILE_MEMORY_KEY, sizeof(char), IPC_CREAT | 0666);

  filename = (char *)shmat(filenameSHMID, NULL, 0);
  fileDir = (char *)shmat(fileDirSHMID, NULL, 0);
  NEW_ACCESS = (int*)shmat(newAccessSHMID, NULL, 0);
  USER_COUNT = (int*)shmat(userCountSHMID, NULL, 0);

  *NEW_ACCESS = 0;

  do {
    if(*NEW_ACCESS == 1){
      if(!strcmp(filename, "users"))
        strcpy(fileDir, "../database/users.txt");
      else if(!strcmp(filename, "cart"))
        strcpy(fileDir, "../database/cart.txt");
      else if(!strcmp(filename, "products"))
        strcpy(fileDir, "../database/products.txt");

      printf("\x1b[33mSolicitud de acceso a: %s, %s\x1b[0m\n", fileDir, filename);

      *NEW_ACCESS = 0;
    }else if(*NEW_ACCESS == 2){
      printf("\n\x1b[31mUsuario desconectado!\x1b[0m\n");
      *USER_COUNT -= 1;
      *NEW_ACCESS = 0;

      if(*USER_COUNT == 0)
        break;
    }
  } while (1);

  shmdt(filename);
  shmdt(fileDir);
  shmdt(NEW_ACCESS);
  shmctl(fileDirSHMID, IPC_RMID, NULL);
  shmctl(filenameSHMID, IPC_RMID, NULL);
  shmctl(newAccessSHMID, IPC_RMID, NULL);
}