#include <stdio.h>
#include <stdint.h>
#include <string.h>

void main(){
    char msg[20] = "12345";

    for(int i = 0; i < 20; i++)
        msg[i] >>= 1;

    printf("%s", msg);
}