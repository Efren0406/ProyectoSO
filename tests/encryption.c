#include "encryption.h"

char* encrypt_password(char* password){
    for(int i = 0; i < 20; i++)
        password[i] >>= 1;
}