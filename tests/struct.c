#include <stdio.h>

typedef struct {
	char name[20];
	char password[20];
	int ID;
} Student;

void main(){
	FILE *fp = fopen("data.dat", "w");
	
	Student papoi = {.name = "efren", .password = "123", .ID = 148};

	fwrite(&papoi, sizeof(Student), 1, fp);

	fclose(fp);
}
