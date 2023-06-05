#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
	char name[20];
	char description[100];
	int ID;
	int price;
} Product;

void main(){
	FILE *fp = fopen("../V2/products.txt", "w");
	Product aux;

	srand(time(NULL));

	for(int i = 0; i < 11; i++){
		sprintf(aux.name, "Producto%d", i + 1);
		sprintf(aux.description, "Descricion del producto %d", i + 1);
		aux.ID = i + 1;
		aux.price = rand() % 50 + 1;
		
		fwrite(&aux, sizeof(Product), 1, fp);
		
		if(fwrite != 0)
			printf("Product %d write succesfully\n", i);
		else
			printf("Product %d failed\n", i);
	}
}
