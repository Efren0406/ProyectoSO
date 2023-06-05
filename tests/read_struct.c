#include <stdio.h>

typedef struct{
	char name[20];
	char password[20];
	int ID;
} Student;

typedef struct {
    int ID;
    int quantity;
} Item;

void main(){
	FILE *fp = fopen("cart.txt", "w");

	fclose(fp);
}
