#include <stdio.h>
#include <string.h>
#include "encryption.h"

typedef struct {
    char user_name[20];
    char password[20];
    int ID_cart;
} User;

void main(){
    FILE *fp = fopen("../V3/database/users.txt", "w");
    char aux[20];

    strcpy(aux, "123");
    encrypt_password(aux);
    printf("%s\n", aux);
    User user1 = {.user_name = "efren", .ID_cart = 1};
    strcpy(user1.password, aux);
    strcpy(aux, "1234");
    encrypt_password(aux);
    printf("%s\n", aux);
    User user2 = {.user_name = "alexis", .ID_cart = 2};
    strcpy(user2.password, aux);
    strcpy(aux, "12345");
    encrypt_password(aux);
    printf("%s\n", aux);
    User user3 = {.user_name = "admin", .ID_cart = -1};
    strcpy(user3.password, aux);

    fwrite(&user3, sizeof(User), 1,  fp);
    fwrite(&user1, sizeof(User), 1,  fp);
    fwrite(&user2, sizeof(User), 1,  fp);

    if(fwrite != 0)
        printf("Database created succesfully");
    else
        printf("Database creation failed");

    fclose(fp);
}
