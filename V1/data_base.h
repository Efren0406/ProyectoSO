#ifndef DATA_BASE_H_
#define DATA_BASE_H_

#include "menus.h"

typedef struct{
  int ID;
  char *name;
  char *description;
  int price;
} Product;

Product* get_product(Product* current_product, int ID);

int* get_cart(int ID);
int create_cart();
int add_cart_item(int Cart_ID, int Product_ID, int quantity);

int get_product_count();
int get_cart_count();

int get_cart_item_count();
int get_user(User *users, int numUsers, const char *username, const char *password);
int readUsersFromFile(User *users);

#endif
