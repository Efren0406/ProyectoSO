#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "../lib/data_base.h"
#include "../lib/server.h"

FILE* requestDB(const char *nombretxt){
  int fileSHMID, filenameSHMID;
  FILE* fp;
  char* DB_name;
  sem_t* SEM;

  SEM = sem_open(CLIENT_SEMAPHORE_NAME, 0600, 0);

  filenameSHMID = shmget(FILENAME_MEMORY_KEY, sizeof(char), IPC_CREAT | 0666);
  fileSHMID = shmget(FILE_MEMORY_KEY, sizeof(FILE), IPC_CREAT | 0666);

  DB_name = (char*)shmat(filenameSHMID, NULL, 0);
  fp = (FILE*)shmat(fileSHMID, NULL, 0);

  strcpy(DB_name, nombretxt);

  sem_post(SEM);
  
  return fp;
}

int get_user(User* current_user, const char* user_name, const char* password){
    FILE* fp = requestDB("../database/users.txt");
  
    if(VALID_PTR(fp)){
         fprintf(stderr, "Error while opening user database!\n");
         exit(1);
    }

    while(fread(current_user, sizeof(User), 1, fp)){
        if(!strcmp(current_user->user_name, user_name) && !strcmp(current_user->password, password)){
            if(!strcmp(current_user->user_name, "admin")){
                fclose(fp);
                return -1;
            }
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

Product* get_product(Product* current_product, int ID){	
	FILE* fp = requestDB("../database/products.txt");

    if(VALID_PTR(fp)){
        fprintf(stderr, "Error while opening product database!\n");
        exit(1);
    }

	while(fread(current_product, sizeof(Product), 1, fp)){
		if(current_product->ID == ID)
			break;
	}

	fclose(fp);
};

int get_product_count(){
    FILE* fp = requestDB("../database/products.txt");
    Product aux;
    int count = 0;

    while(fread(&aux, sizeof(Product), 1, fp)){
        count++;
    }

    fclose(fp);

    return count;
}

int get_cart_count(const int ID_cart){
    FILE* fp = requestDB("../database/cart.txt");

    if(VALID_PTR(fp)){
        fprintf(stderr, "Error while opening cart database!\n");
        exit(1);
    }

    int count = 0;
    cart_Item aux;

    while(fread(&aux, sizeof(cart_Item), 1, fp)){
        if(aux.cart_ID == ID_cart)
            count++;
    }
    
    fclose(fp);
    return count;
}

void add_cart_item(const int ID_cart, const int Item_ID, const int quantity){
    FILE* fp = requestDB("../database/cart.txt");
    FILE *temp = fopen("../database/temp.txt", "a");
    cart_Item aux;
    int found = 0;

    while(fread(&aux, sizeof(cart_Item), 1, fp)){
        if(aux.cart_ID == ID_cart && aux.product_ID == Item_ID){
            aux.quantity += quantity;
            found = 1;

            if(aux.quantity <= 0)
                continue;
        }

        fwrite(&aux, sizeof(cart_Item), 1, temp);
    }
    
    if(!found && quantity > 0){
        aux.cart_ID = ID_cart;
        aux.product_ID = Item_ID;
        aux.quantity = quantity;

        fwrite(&aux, sizeof(cart_Item), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("../database/cart.txt");
    rename("../database/temp.txt", "../database/cart.txt");
}

int get_user_ID_cart(const char *user_name){
    FILE* fp = requestDB("../database/users.txt");
    User current_user;

    while(fread(&current_user, sizeof(User), 1, fp)){
        if(!strcmp(current_user.user_name, user_name)){
            fclose(fp);
            return current_user.ID_cart;
        }
    }

    fclose(fp);
}

void delete_cart(const char* name){
    int ID_cart = get_user_ID_cart(name);
    FILE* fp = requestDB("../database/cart.txt");
    FILE *temp = fopen("../database/temp.txt", "a");
    cart_Item aux;

    while(fread(&aux, sizeof(cart_Item), 1, fp)){
        if(aux.cart_ID == ID_cart){
            continue;
        }

        fwrite(&aux, sizeof(cart_Item), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("../database/cart.txt");
    rename("../database/temp.txt", "../database/cart.txt");
}

int* get_cart_item(const int cart_ID, int* item_ID, int* quantity, int index){
    FILE *fp = fopen("../database/cart.txt", "r");
    cart_Item aux;

    int i = 0;

    while(fread(&aux, sizeof(cart_Item), 1, fp)){
        if(cart_ID == aux.cart_ID){
            if(i == index){
                *item_ID = aux.product_ID;
                *quantity = aux.quantity;
                break;
            }else
                i++;
        }
    }

    fclose(fp);
}	

void add_product(const int ID, const char* name, const char* description, const int price){
    FILE* fp = requestDB("../database/products.txt");
    FILE *temp = fopen("../database/temp.txt", "a");
    Product aux;
    int found = 0;
    char option;

    while(fread(&aux, sizeof(Product), 1, fp)){
        if(aux.ID == ID){
            printf("El producto ya existe, desea actualizarlo? [y/n]\n");
            scanf("%c", &option);

            if(option == 'y'){
                strcpy(aux.name, name);
                strcpy(aux.description, description);
                aux.price = price;

                found = 1;
            }
        }else if(aux.ID == -ID)
            continue;

        fwrite(&aux, sizeof(Product), 1, temp);
    }

    if(!found && ID > 0){
        aux.ID = ID;
        strcpy(aux.name, name);
        strcpy(aux.description, description);
        aux.price = price;

        fwrite(&aux, sizeof(Product), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("../database/products.txt");
    rename("../database/temp.txt", "../database/products.txt");
}

void add_user(const char* name, const char* password){
    FILE* fp = requestDB("../database/users.txt");
    FILE *temp = fopen("../database/temp.txt", "a");
    User aux;
    int found = 0, deleted = 0;
    char option;

    while(fread(&aux, sizeof(User), 1, fp)){
        if(!strcmp(aux.user_name, name) && strcmp(password, "delete")){
            printf("El Usuario ya existe, desea cambiar la contrase%ca? [y/n]\n", 164);
            scanf("%c", &option);

            if(option == 'y'){
                strcpy(aux.password, password);
                found = 1;
            }
        }else if(!strcmp(aux.user_name, name) && !strcmp(password, "delete") && !deleted){
            deleted = 1;
            continue;
        }

        fwrite(&aux, sizeof(User), 1, temp);
    }

    if(!found && !deleted && strcmp(password, "delete")){
        strcpy(aux.user_name, name);
        strcpy(aux.password, password);
        aux.ID_cart += 1;

        fwrite(&aux, sizeof(User), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("../database/users.txt");
    rename("../database/temp.txt", "../database/users.txt");
}