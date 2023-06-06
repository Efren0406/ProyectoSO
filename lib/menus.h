#ifndef MENUS_H_
#define MENUS_H_

#include <ncurses.h>

int login(WINDOW* window);
int menu(WINDOW* window);
int cart(WINDOW* window);

// Pantalla de pago pendiente
// int purchase(WINDOW* window);

int admin_menu(WINDOW* window);

#endif