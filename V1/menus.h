#ifndef MENUS_H_
#define MENUS_H_

#define USER_SIZE 20
#define PASSWORD_SIZE 20

typedef struct{
  char *user_name;
  char *password;
  int ID_cart;
} User;

typedef struct{
  int ID;
  int quantity;
} Item;

typedef struct{
  int ID;
  int count;
  Item *item[100];
} Cart;

int login();
int menu();
int cart();
int purchase();

#endif
