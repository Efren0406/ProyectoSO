#include "menus.h"
#include <stdio.h>
#include <stdlib.h>

int main() {    
  int state = 0;
  
  while(state != -1){
    if(state == 0)
      state = login();
    else if(state == 1)
      state = menu();
    else if(state == 2)
      state = cart();
    else if(state == 3)
      state = purchase();
  }
  
  return 0;
}