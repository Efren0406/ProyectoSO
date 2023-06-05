#include "../lib/encryption.h"
#include "../lib/const.h"

char* encrypt_password(char* password){
    for(int i = 0; i < PASSWORD_SIZE; i++)
        password[i] >>= 1;
}