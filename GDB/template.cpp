#include <iostream>

template <typename T>
class smart_pointer
{
private:
  T* _pointer;
public:
  smart_pointer(T* pointer) : _pointer(pointer)
  {
  }
  T* get()
  {
    return _pointer;
  }
};

int main(int argc, char const *argv[]) {
  int value=150;
  smart_pointer<int> ptr(&value);

  int ret = (*(ptr.get()));

  return 0;
}
