#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x;
  int y;
} position_t;

int main(int argc, char const *argv[]) {

  position_t* position=(position_t*)malloc(sizeof(position_t));
  position->x=105;
  position->y=205;

  while(position->x < 110) {
    position->x++;
  }

  return 0;
}
