#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../lib/const.h"
#include "../lib/data_base.h"
#include "../lib/encryption.h"
#include "../lib/menus.h"
#include "../lib/server.h"

User current_user;
int adminFlag;

void serverListener(void){
  int myFlag = 1, adminUpdateSHMID;
  int *ADMIN_UPDATE;
  char c;

  adminUpdateSHMID = shmget(ADMIN_UPDATE_KEY, sizeof(int), IPC_CREAT | 0666);
  ADMIN_UPDATE = (int*)shmat(adminUpdateSHMID, NULL, 0);

  while(1){
    if(*ADMIN_UPDATE > 0 && myFlag && !adminFlag){
      printf("\n\n\x1b[93mEl administrador actualizo la base de datos!\n");
      printf("Puede que lo mostrado en pantalla no este actualizado!\n");
      printf("Presione <Enter> para actualizar\x1b[0m\n");

      *ADMIN_UPDATE -= 1;
      myFlag = 0;
    }

    if(*ADMIN_UPDATE == 0){
      myFlag = 1;
    }

    if(*ADMIN_UPDATE == -2){
      printf("\n\n\n\x1b[91m***El servidor ha sido apagado, intentelo más tarde!***\x1b[0m\n");
      exit(1);
    }
  }
}

int login() {
  char input_User_name[USER_SIZE + 1];
  char input_Password[PASSWORD_SIZE + 1];
  int login_check = 0, c;

  initSH();

  system("clear");

  while (1) {
    printf("\x1b[93m=============== Iniciar Sesion ================\x1b[0m\n");
    printf("\x1b[94mIngrese el nombre de usuario: \x1b[0m");
    scanf("%s", input_User_name);

    if(strlen(input_User_name) > USER_SIZE){
      while((c = getchar()) != '\n' && c != EOF);
      system("clear");
      printf("\x1b[91mEl nombre de usuario no debe superar los 20 caracteres!\x1b[0m\n\n");
      continue;
    }

    printf("\x1b[94mIngrese la contrase%ca: \x1b[0m", 164);
    scanf("%s", input_Password);

    if(strlen(input_Password) > PASSWORD_SIZE){
      while((c = getchar()) != '\n' && c != EOF);
      system("clear");
      printf("\x1b[91mLa contraseña no debe superar los 20 caracteres!\x1b[0m\n\n");
      continue;
    }

    encrypt_password(input_Password);

    login_check = get_user(&current_user, input_User_name, input_Password);

    if (login_check == 1 || login_check == -1) {
      system("clear");
      return login_check;
    } else {
      system("clear");
      printf("\x1b[92mUsuario o contraseña incorrectos D:\x1b[0m\n\n");
    }
  }
}

int menu() {
  char option;
  int first = 1, interval = 10;
  int total;
  int ID, quantity;

  adminFlag = 0;

  while (1) {
    Product *list_product = (Product *)malloc(sizeof(Product));
    total = get_product_count();

    printf("\x1b[96m]=============== Bienvenido %s a IC Store ===============\x1b[0m\n", current_user.user_name);
    printf("Articulos %d - %d / %d\n", first, first + interval - 1, total);

    for (int i = first - 1; i < first + interval - 1; i++) {
      if (i < total){
        get_nth_product(list_product, i);
        printf("%d  %s  %s  $%d\n", list_product->ID, list_product->name, list_product->description, list_product->price);
      }
    }

    printf("\n1. Agregar al carrito\n");
    printf("2. Ir al carrito\n");
    printf("3. Siguiente pagina\n");
    printf("4. Pagina Anterior\n");
    printf("\x1b[31m5. Cerrar sesion\x1b[0m\n");

    option = getchar();
    getchar();

    if (option == '1') {
      system("clear");
      printf("Indique el ID del producto a agregar: ");
      scanf("%d", &ID);
      printf("Indique la cantidad: ");
      scanf("%d", &quantity);

      getchar();

      get_product(list_product, ID);
      if(ID > 0 && quantity > 0) {
        if (list_product->ID == ID) {
          system("clear");
          add_cart_item(current_user.ID_cart, ID, quantity);
        } else {
          system("clear");
          printf("\n\x1b[91mEl producto que se indico no existe, intentar de nuevo!\x1b[0m\n");
        }
      }else{
        system("clear");
        printf("\x1b[91mEl ID y la cantidad deben ser valores positivos!\x1b[0m\n\n");
      }
    } else if (option == '2') {
      system("clear");
      getchar();
      return 2;
    } else if (option == '3' && first + interval - 1 < total) {
      system("clear");
      first += interval;
    } else if (option == '4' && first > 1) {
      system("clear");
      first -= interval;
    } else if (option == '5')
      return 0;
    else
      system("clear");

    free(list_product);
  }
}

int cart() {
  int total;
  int total_cart_amount;
  int ID, quantity;
  char option;

  while (1) {
    total = get_cart_count(current_user.ID_cart);
    printf("\x1b[96m=============== Tu carrito %s ===============\x1b[0m\n", current_user.user_name);

    if (total > 0) {
      Product *cart_item = (Product *)malloc(sizeof(Product));
      total_cart_amount = 0;

      for (int i = 0; i < total; i++) {
        get_cart_item(current_user.ID_cart, &ID, &quantity, i);
        get_product(cart_item, ID);
        total_cart_amount += cart_item->price * quantity;
        printf("%d  %s  $%d | %d pzs.\n", cart_item->ID, cart_item->name, cart_item->price, quantity);
      }

      printf("\n\t\tTotal: $%d\n", total_cart_amount);

      printf("\n1. Eliminar producto\n");
      printf("2. Regresar al menu principal\n");

      option = getchar();

      if (option == '1') {
        printf("\nIndique el ID del producto a eliminar: ");
        scanf("%d", &ID);
        printf("\nIndique la cantidad a eliminar: ");
        scanf("%d", &quantity);
        getchar();

        get_product(cart_item, ID);
        system("clear");

        if(ID > 0 && quantity > 0){
          if (cart_item->ID == ID) {
            add_cart_item(current_user.ID_cart, ID, -quantity);
          } else {
            printf("\n\x1b[91mEl producto que indico no existe, intentar de nuevo!\x1b[0m\n");
          }
        }else{
          system("clear");
          printf("\x1b[91mEl ID y la cantidad deben ser valores positivos!\x1b[0m\n\n");
        }
      } else if (option == '2') {
        getchar();
        return 1;
      }else
        system("clear");

      free(cart_item);
    } else {
      printf("\x1b[93mTu carrito esta vacio!\nAgrega productos en el menu principal\x1b[0m\n");

      printf("\nPresione <Enter> para regresar\n");

      option = getchar();

      if (option == '\n')
        return 1;
    }
  }
}

int admin_menu() {
  int access = 0;
  int ID, price;
  int adminUpdateSHMID;
  int *ADMIN_UPDATE;
  char product_name[21], description[101];
  char user_name[USER_SIZE], password[PASSWORD_SIZE];
  char option;

  adminFlag = 1;

  adminUpdateSHMID = shmget(ADMIN_UPDATE_KEY, sizeof(int), IPC_CREAT | 0666);
  ADMIN_UPDATE = (int*)shmat(adminUpdateSHMID, NULL, 0);

  while (1) {
    printf("=============== Panel de Administrador ===============\n");
    printf("Opciones: \n\n");
    printf("1. Agregar producto\n");
    printf("2. Agregar Usuario\n");
    printf("3. Eliminar producto\n");
    printf("4. Eliminar Usuario\n");
    printf("5. Salir del panel de Administrador\n");

    option = getchar();

    if (option == '1') {
      system("clear");
      printf("Ingrese el ID del producto: ");
      scanf("%d", &ID);
      printf("Ingrese el precio del producto: ");
      scanf("%d", &price);
      getchar();

      if(ID < 0 || price < 0){
        system("clear");
        printf("\x1b[91mEl ID y el precio deben ser valores positivos!\x1b[0m\n\n");
        continue;
      }

      printf("Ingrese el nombre del producto [20 caracteres max]: ");
      scanf("%[^\n]*c", product_name);
      getchar();

      if(strlen(product_name) > USER_SIZE){
        system("clear");
        printf("\x1b[91mEl nombre del producto no debe ser mayor a 20 caracteres!\x1b[0m\n\n");
        continue;
      }

      printf("Ingrese la descripcion del producto [100 caracteres max]: ");
      scanf("%[^\n]*c", description);
      getchar();

      if(strlen(product_name) > USER_SIZE){
        system("clear");
        printf("La descripcion del producto no debe superar los 100 caracteres!\n\n");
        continue;
      }

      add_product(ID, product_name, description, price);
      system("clear");
      printf("\x1b[92mProducto agregado exitosamente a la base de datos!\x1b[0m\n\n");

      *ADMIN_UPDATE = -1;
    } else if (option == '2') {
      system("clear");
      printf("Ingrese el nombre del Usuario [%d caracteres max]: ", USER_SIZE);
      scanf("%s", user_name);
      getchar();

      if(strlen(user_name) > USER_SIZE){
        system("clear");
        printf("\x1b[91mEl nombre de usuario no debe superar los 20 caracteres!\x1b[0m\n\n");
        continue;
      }

      printf("Ingrese la contrase%ca [%d caracteres max]: ", 164, PASSWORD_SIZE);
      scanf("%s", password);
      getchar();

      if(strlen(password) > USER_SIZE){
        system("clear");
        printf("\x1b[91mLa contraseña no debe superar los 20 caracteres!\x1b[0m\n\n");
        continue;
      }

      encrypt_password(password);

      add_user(user_name, password);

      system("clear");
      printf("\x1b[92mUsuario agregado exitosamente a la base de datos!\x1b[0m\n\n");
    } else if (option == '3') {
      system("clear");
      printf("Ingrese el ID del producto: ");
      scanf("%d", &ID);
      getchar();

      if(ID < 0){
        system("clear");
        printf("\x1b[91mEl ID debe ser un valor positivo!\x1b[0m\n\n");
        continue;
      }

      add_product(-ID, "", "", 0);

      system("clear");
      printf("\x1b[92mProducto eliminado exitosamente!\x1b[0m\n\n");

      *ADMIN_UPDATE = -1;
    } else if (option == '4') {
      system("clear");
      printf("Ingrese el nombre del usuario: ");
      scanf("%s", user_name);
      getchar();

      if(strlen(user_name) > USER_SIZE){
        system("clear");
        printf("\x1b[91mEl nombre de usuario no debe superar los 20 caracteres!\x1b[0m\n\n");
        continue;
      }

      delete_cart(user_name);
      add_user(user_name, "delete");

      system("clear");
      printf("\x1b[92mUsuario eliminado exitosamente!\x1b[0m\n\n");

      *ADMIN_UPDATE = -1;
    } else if (option == '5'){
      adminFlag = 0;
      return 0;
    }
    else
      system("clear");
  }
}