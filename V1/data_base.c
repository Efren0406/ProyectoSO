#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "data_base.h"
#include "menu.h"

Product* get_product(Product* current_product, int ID){	
	FILE *fp = fopen("products.txt", "r");
	int i = 0;
	char line[300];

	while(fgets(line, sizeof(line), fp) != NULL){
		if(i != 0){
			current_product->ID = atoi(strtok(line, ","));
			current_product->name = strtok(NULL, ",");
			current_product->description = strtok(NULL, ",");	
			current_product->price = atoi(strtok(NULL, ","));

			if(current_product->ID == ID)
				break;
		}

		i++;
	}

	fclose(fp);

	return current_product;
};

int create_cart(int user_ID){
	int ID;	
	int i = 1;
	char line[50];
	char aux[30];
	char *token;

	FILE *fp = fopen("cart.txt", "a");
	fprintf(fp, "0,\n");
	fclose(fp);

	fp = fopen("cart.txt", "r+");
	fgets(line, 50, fp);
	ID = atoi(line) + 1;
	fseek(fp, 0, SEEK_SET);
	fprintf(fp, "%d\n", ID);
	fclose(fp);

	fp = fopen("usuarios.txt", "r+");

	while(fgets(line, 50, fp) != NULL){
		if(i == user_ID){
			token = strtok(line, " \t\n");
			strcpy(aux, token);
			strcat(aux, " ");
			token = strtok(NULL, " \t\n");
			strcat(aux, token);

			strcpy(line, aux);
			sprintf(aux, " %d", ID);
			strcat(line, aux);
			fseek(fp, -strlen(line), SEEK_CUR);
			fprintf(fp, "%s", line);

			break;
		}

		i++;
	}

	fclose(fp);

	return ID;
};

int add_cart_item(int Cart_ID, int Product_ID, int quantity){
	FILE *fp = fopen("cart.txt", "r+");
	int i = 1;
	int item_count;
	char *line = (char*)malloc(sizeof(char) * 300);
	char * items = (char*)malloc(sizeof(char) * 100);
	char *aux = (char*)malloc(sizeof(char) * 10);

	while(fgets(line, sizeof(line), fp) != NULL){
		if(i == Cart_ID + 1){
			sscanf(line, "%d,%[^\n]", &item_count, items);

			strcat(items, ",");

			if(item_count == 0)
				strcpy(items, "");

			sprintf(aux, "%d,", Product_ID);
			strcat(items, aux);
			sprintf(aux, "%d", quantity);
			strcat(items, aux);


			fseek(fp, -strlen(line), SEEK_CUR);
			fprintf(fp, "%d,%s\n", item_count + 1, items);

			break;
		}

		i++;
	}

	free(line);
	free(items);
	free(aux);
	fclose(fp);
};

int get_product_count(){
	FILE *fp = fopen("products.txt", "r");
	char line[10];

	fgets(line, sizeof(line), fp);

	int count = atoi(line);

	fclose(fp);

	return count;
};

int get_cart_count();