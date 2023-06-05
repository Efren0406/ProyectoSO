#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "menus.h"
#include "data_base.h"

User current_user = {.user_name = "", .password = "", .ID_cart = 0};
int user_ID;

int get_user(User *users, int numUsers, const char *username, const char *password) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].user_name, username) == 0 && strcmp(users[i].password, password) == 0) {
            current_user.user_name = users[i].user_name;
            current_user.ID_cart = users[i].ID_cart;
            return i + 1;  // Inicio de sesión exitoso
        }
    }
    return 0;  // Inicio de sesión fallido
}

int readUsersFromFile(User *users) {
    FILE *file = fopen("usuarios.txt", "r");

    int numUsers = 0;
    char linea[USER_SIZE + PASSWORD_SIZE + 10];
    char *username;
    char *password;
    char *ID_cart_str;

    while (fgets(linea, sizeof(linea), file) != NULL) {
        username = strtok(linea, " \t\n");
        password = strtok(NULL, " \t\n");
        ID_cart_str = strtok(NULL, " \t\n");

        if (username != NULL && password != NULL) {
            users[numUsers].user_name = malloc(strlen(username) + 1);  // Reserva memoria para el nombre de usuario
            users[numUsers].password = malloc(strlen(password) + 1);  // Reserva memoria para la contraseña

            strcpy(users[numUsers].user_name, username);
            strcpy(users[numUsers].password, password);

            users[numUsers].ID_cart = atoi(ID_cart_str);

            numUsers++;
        }
    }

    fclose(file);
    return numUsers;
}

int login() {
    User *users = malloc(100 * sizeof(User));
    char inputUsername[USER_SIZE];
    char inputPassword[PASSWORD_SIZE];
    int numUsers;

    numUsers = readUsersFromFile(users);
    if (numUsers == 0) {
        free(users);
        return -1;
    }

    printf("=============== Iniciar Sesion ================\n");
    printf("Ingrese el nombre de usuario: ");
    scanf("%s", inputUsername);
    printf("Ingrese la contraseña: ");
    scanf("%s", inputPassword);

    user_ID = get_user(users, numUsers, inputUsername, inputPassword);

    if (user_ID != 0) {
        printf("%s, iniciaste sesión :D\n", inputUsername);
        return 1;
    } else {
        printf("Usuario o contraseña incorrectos D:\n");
        return 0;
    }

    for (int i = 0; i < numUsers; i++) {
        free(users[i].user_name);  // Libera la memoria del nombre de usuario
        free(users[i].password);   // Libera la memoria de la contraseña
    }

    free(users);  // Libera la memoria del arreglo de usuarios
};

int menu(){
  Product *list_product = (Product *) malloc(sizeof(Product));
  Cart *user_cart = (Cart *) malloc(sizeof(Cart));
  char option;
  int first_product = 1, interval = 10;
  int total_products = get_product_count();
  int ID, quantity;

  while(1){
    printf("=============== Bienvenido %s %d===============\n", current_user.user_name, user_ID);
    printf("Articulos %d - %d / %d\n", first_product, first_product + interval - 1, total_products);

    for(int i = 0; i < 10; i++){
      get_product(list_product, i + first_product);
      if(i + first_product <= total_products)
        printf("%d  %s  %s  %d\n", list_product->ID, list_product->name, list_product->description, list_product->price);
    }

    printf("\n1. Agregar al carrito\n");
    printf("2. Ir al carrito\n"); 
    if(first_product == 1 && total_products > first_product + interval - 1){
        printf("3. Siguiente pagina\n");
        printf("4. Cerrar sesion\n");
    }else if(first_product == 1 && !(total_products > first_product + interval - 1))
        printf("3. Cerrar sesion\n");
    else if(first_product > 1 && total_products > first_product + interval - 1){
        printf("3. Siguiente pagina\n");
        printf("4. Pagina anterior\n");
        printf("5. Cerrar sesion\n");
    }else if(first_product > 1 && !(total_products > first_product + interval - 1)){
        printf("3. Pagina anterior\n");
        printf("4. Cerrar sesion\n");
    }

    option = getchar();

    if(option == '1'){
      system("clear");
      printf("Indique el ID del producto a agregar: ");
      scanf("%d", &ID);
      printf("Indique la cantidad: ");
      scanf("%d", &quantity);

      if(current_user.ID_cart == 0){
        current_user.ID_cart = create_cart(user_ID);
        add_cart_item(current_user.ID_cart, ID, quantity);
      }else
        add_cart_item(current_user.ID_cart, ID, quantity);
    }else if(option == '2'){
      system("clear");
      return 2;
    }else if(option == '3')
      if(total_products > first_product + interval - 1){
          first_product += interval;
      }else if(first_product == 1 && !(total_products > first_product + interval - 1))
          return 0;
      else if(first_product > 1 && !(total_products > first_product + interval - 1)){
          first_product -= interval;
      }
    else if(option == '4'){
      if(first_product == 1 || !(total_products > first_product + interval - 1))
          return 0;
      else if(first_product > 1 && total_products > first_product + interval - 1)
          first_product -= interval;
    }else if(option == '5' && total_products > first_product + interval - 1 && first_product > 1)
        return 0;

    system("clear");
  }

  free(list_product);

  return 1;
};

int cart(){return 1;};

int purchase(){return 1;};
