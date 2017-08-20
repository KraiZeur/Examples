#include <iostream>

template<typename T>
class VirtualTemplateBase {
  template <typename P>
  void f(P& p) {
    if (dynamic_cast<T>(this))
      (dynamic_cast<T>(this))->f(p);
  }
};

class VirtualTemplateConcrete : public VirtualTemplateBase<VirtualTemplateConcrete> {
  template <typename P>
  void f(P& p) {
    std::cout << p << std::endl;
  }
};

int main(int argc, char const *argv[]) {

  VirtualTemplateConcrete ctr;
  ctr.f<int>(5);

  return 0;
}
