struct A {
  int value;
  char value2;
  double value3;
  char value4;
};

int main(int argc, char const *argv[]) {

  struct A a;
  a.value=50;

  int x=a.value*a.value;

  return 0;
}
