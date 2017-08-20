#include <iostream>
#include <string>

std::string values[10] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J" };

void call(int i, std::string reference) {
  int x=i*2;
  x=x+(int)reference[0];
}

int main(int argc, char const *argv[]) {

  for (int i = 0; i < 10; i++) {
    call(i, values[i]);
  }

  return 0;
}
