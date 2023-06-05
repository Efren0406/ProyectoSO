#ifndef CONST_H_
#define CONST_H_

#define VALID_PTR(x) ((x) == NULL)

#define WINDOW_HEIGHT LINES - 5
#define WINDOW_WIDTH COLS - 5

#define USER_SIZE   20
#define PASSWORD_SIZE   20

typedef struct {
    char user_name[20];
    char password[20];
    int ID_cart;
} User;

typedef struct{
  char name[20];
  char description[100];
  int ID;
  int price;
} Product;

typedef struct{
    int cart_ID;
    int product_ID;
    int quantity;
} cart_Item;

#endif
