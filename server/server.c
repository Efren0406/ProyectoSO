#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "../lib/server.h"
#include "../lib/const.h"

void userListener(void* userNum){
    int requestSHMID, userSHMID, productSHMID, itemSHMID;
    int SEMnum;
    // Estructuras y variables que utiliza el usuario
    // para acceder a la base de datos
    // La variable "request", recibe el identificador
    // de la solicitud que hace el usuario
    // Ej. Contraseñá, productos de la tienda, productos
    // del carrito, etc.
    char *request, *SEMname;
    User *currentUser;
    Product *currentProduct;
    cart_Item *currentItem;

    sem_t *SEM;

    sprintf(SEMname, "%d", userNum);

    SEM = sem_open(SEMname, O_CREAT, 0600, 0);

    requestSHMID = shmget(SHARED_MEMORY_KEY, 2, IPC_CREAT | 0666);
    userSHMID = shmget(SHARED_MEMORY_KEY, sizeof(User), IPC_CREAT | 0666);
    productSHMID = shmget(SHARED_MEMORY_KEY, sizeof(Product), IPC_CREAT | 0666);
    itemSHMID = shmget(SHARED_MEMORY_KEY, sizeof(cart_Item), IPC_CREAT | 0666);

    request = (char*)shmat(requestSHMID, NULL, 0);
    currentUser = (User*)shmat(userSHMID, NULL, 0);
    currentProduct = (Product*)shmat(productSHMID, NULL, 0);
    currentItem = (cart_Item*)shmat(itemSHMID, NULL, 0);

    while(1){
        // Aqui se implementan el control de las solicitudes de los clientes
    }

    shmdt(currentUser);
    shmdt(currentProduct);
    shmdt(currentItem);
    shmctl(userSHMID, IPC_RMID, NULL);
    shmctl(productSHMID, IPC_RMID, NULL);
    shmctl(itemSHMID, IPC_RMID, NULL);
}