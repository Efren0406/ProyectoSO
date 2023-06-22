#ifndef DATA_BASE_H_
#define DATA_BASE_H_

#include "const.h"

void initSH();

int get_user(User* current_user, const char* user_name, const char* password);

Product* get_product(Product* current_product, int ID);	
Product* get_nth_product(Product* current_product, int i);

int* get_cart_item(const int cart_ID, int* item_ID, int* quantity, int index);	

int get_product_count();
int get_cart_count();

void add_cart_item(const int ID_cart, const int Item_ID, const int quantity);
void add_product(const int ID, const char* name, const char* description, const int price);
void add_user(const char* name, const char* password);

void delete_cart(const char* name);
#endif