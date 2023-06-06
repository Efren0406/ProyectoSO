#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "../lib/menus.h"
#include "../lib/const.h"
#include "../lib/data_base.h"
#include "../lib/encryption.h"

User current_user;

int login(WINDOW *window){
    char input_User_name[USER_SIZE];
    char input_Password[PASSWORD_SIZE];
    int login_check = 0;
    int ch;
    int i;

    while(1){
        wattron(window, COLOR_PAIR(2));
        mvwprintw(window, 5, (COLS - 47) / 2, "=============== Iniciar Sesion ================");
        wattron(window, COLOR_PAIR(1));
        mvwprintw(window, 7, (COLS - 40) / 2, "Ingrese el nombre de usuario: ");
        mvwprintw(window, 9, (COLS - 40) / 2, "Ingrese la contrase%ca: ", 164);
        wmove(window, 7, (COLS + 20) / 2);
        wrefresh(window);

        wattron(window, COLOR_PAIR(5));

        i = 0;

        while ((ch = wgetch(window)) != '\n') {
            if (ch != 127 && ch != 8) {
                if(i < USER_SIZE){
                    wattron(window, COLOR_PAIR(5));
                    input_User_name[i] = ch;
                }else{
                    wattron(window, COLOR_PAIR(3));
                }
                i++;
                waddch(window, ch);
            } else if ((ch == 127 || ch == 8) && i > 0) {
                i--;
                mvwaddch(window, 7, (COLS + 20) / 2 + i, ' ');
                wmove(window, 7, (COLS + 20) / 2 + i);
            }
            wrefresh(window);
        }


        input_User_name[i] = '\0';

        wattron(window, COLOR_PAIR(4));
        mvwprintw(window, 7, (COLS + 20) / 2, "%s", input_User_name);
        wattron(window, COLOR_PAIR(5));
        wmove(window, 9, (COLS + 8) / 2);

        wrefresh(window);

        i = 0;

        while ((ch = wgetch(window)) != '\n') {
            if (ch != 127 && ch != 8) {
                if(i < PASSWORD_SIZE){
                    wattron(window, COLOR_PAIR(5));
                    input_Password[i] = ch;
                }else{
                    wattron(window, COLOR_PAIR(3));
                }
                i++;
                waddch(window, '*');
            } else if ((ch == 127 || ch == 8) && i > 0) {
                i--;
                mvwaddch(window, 9, (COLS + 8) / 2 + i, ' ');
                wmove(window, 9, (COLS + 8) / 2 + i - 1);
            }
            wrefresh(window);
        }

        wattroff(window, COLOR_PAIR(4));

        input_Password[i] = '\0';

        encrypt_password(input_Password);

        login_check = get_user(&current_user, input_User_name, input_Password);

        if(login_check == 1 || login_check == -1)
            return login_check;
        else{
            werase(window);
            wrefresh(window);
            wattron(window, COLOR_PAIR(3));
            mvwprintw(window, 14, (COLS - 35) / 2, "Usuario o contraseña incorrectos D:");
            wattroff(window, COLOR_PAIR(3));
        }
    }
}

int menu(){
    char option;
    int first = 1, interval = 10;
    int total = get_product_count();
    int ID, quantity;

    while(1){
        Product* list_product = (Product*)malloc(sizeof(Product));

        system("clear");
        printf("=============== Bienvenido %s ===============\n", current_user.user_name);
        printf("Articulos %d - %d / %d\n", first, first + interval - 1, total);

        for(int i = 0; i < interval; i++){
            get_product(list_product, i + first);
            if(i + first <= total)
                printf("%d  %s  %s  $%d\n", list_product->ID, list_product->name, list_product->description, list_product->price);
        }

        free(list_product);

        printf("\n1. Agregar al carrito\n");
        printf("2. Ir al carrito\n");
        printf("3. Siguiente pagina\n");
        printf("4. Pagina Anterior\n");
        printf("5. Cerrar sesion\n");

        option = getchar();

        if(option == '1'){
            system("clear");
            printf("Indique el ID del producto a agregar: ");
            scanf("%d", &ID);
            printf("Indique la cantidad: ");
            scanf("%d", &quantity);

            add_cart_item(current_user.ID_cart, ID, quantity);
        }else if(option == '2'){
            getchar();
            return 2;
        }else if(option == '3' && first + interval - 1 < total)
            first += interval;
        else if(option == '4' && first > 1)
            first -= interval;
        else if(option == '5')
            return 0;
    }
}

int cart(){
    int total;
    int total_cart_amount;
    int ID, quantity;
    char option;

    while(1){
        total = get_cart_count(current_user.ID_cart);
        system("clear");
        printf("=============== Tu carrito %s ===============\n", current_user.user_name);

        if(total > 0) {
            Product* cart_item = (Product*)malloc(sizeof(Product));
            total_cart_amount = 0;

            for(int i = 0; i < total; i++){
                get_cart_item(current_user.ID_cart, &ID, &quantity, i);
                get_product(cart_item, ID);
                total_cart_amount += cart_item->price * quantity;
                printf("%d  %s  $%d | %d pzs.\n", cart_item->ID, cart_item->name, cart_item->price, quantity);
            }

            free(cart_item);

            printf("\n\t\tTotal: $%d\n", total_cart_amount);

            printf("\n1. Comprar todo\n");
            printf("2. Eliminar producto\n");
            printf("3. Regresar al menu principal\n");

            option = getchar();

            if(option == '1'){
                return 3;
            }else if(option == '2'){
                printf("\nIndique el ID del producto a eliminar: ");
                scanf("%d", &ID);
                printf("\nIndique la cantidad a eliminar: ");
                scanf("%d", &quantity);
                getchar();

                add_cart_item(current_user.ID_cart, ID, -quantity);
            } else if(option == '3')
                return 1;
        }else{
            printf("Tu carrito esta vacio!\nAgrega productos en el menu principal\n");

            printf("\nPresione <Enter> para regresar\n");

            option = getchar();

            if(option == '\n')
                return 1;
        }
    }
}

int purchase(){
    return 0;
}

int admin_menu(){
    int access = 0;
    int ID, price;
    char product_name[20], description[100];
    char user_name[USER_SIZE], password[PASSWORD_SIZE];
    char option;

    while(1){
        system("clear");
        printf("=============== Panel de Administrador ===============\n");
        printf("Opciones: \n\n");
        printf("1. Agregar producto\n");
        printf("2. Agregar Usuario\n");
        printf("3. Eliminar producto\n");
        printf("4. Eliminar Usuario\n");
        printf("5. Salir del panel de Administrador\n");

        option = getchar();

        if(option == '1'){
            system("clear");
            printf("Ingrese el ID del producto: ");
            scanf("%d", &ID);
            printf("Ingrese el precio del producto: ");
            scanf("%d", &price);
            getchar();
            printf("Ingrese el nombre del producto [20 caracteres max]: ");
            scanf("%[^\n]*c", product_name);
            getchar();
            printf("Ingrese la descripcion del producto [100 caracteres max]: ");
            scanf("%[^\n]*c", description);
            getchar();

            add_product(ID, product_name, description, price);
        }else if(option == '2'){
            system("clear");
            printf("Ingrese el nombre del Usuario [%d caracteres max]: ", USER_SIZE);
            scanf("%s", user_name);
            getchar();
            printf("Ingrese la contrase%ca [%d caracteres max]: ", 164, PASSWORD_SIZE);
            scanf("%s", password);
            getchar();

            encrypt_password(password);

            add_user(user_name, password);
        }else if(option == '3'){
            system("clear");
            printf("Ingrese el ID del producto: ");
            scanf("%d", &ID);
            getchar();

            add_product(-ID, "", "", 0);
        }else if(option == '4'){
            system("clear");
            printf("Ingrese el nombre del usuario: ");
            scanf("%s", user_name);

            delete_cart(user_name);
            add_user(user_name, "delete");
        }else if(option == '5')
            return 0;
    }
}