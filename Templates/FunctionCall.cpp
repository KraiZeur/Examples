/**
 * It mostly demonstrate how to pass function as parameter using templates
 */

#include <iostream>
#include <typeinfo>

class Base {
private:
public:
    void doIt() {
        std::cout << "Do IT from Member function" << std::endl;
    }
};

/**
 * Function call
 */
void doIt() {
  std::cout << "Do It" << std::endl;
}

template <typename A, typename B>
void doItArgs(A a, B b) {
  std::cout << "Do It with args " << typeid(a).name() << sizeof(A) << " " <<  typeid(b).name() << sizeof(B) << std::endl;
}

template <typename T>
void function(T fn) {
  fn();
}

template <typename T, typename A, typename B>
void functionArgs(A a, B b, T (func) (A,B)) {
  func(a,b);
}


/**
 * Member function call
 */
template <class C>
void function(void (C::*p)(), C* owner) {
    (owner->*p)();
}

int main(void) {
  /**
   * Compiler will resolve automatically template description :
   * T = void (*)()
   */
  function(&doIt);

  /**
   * Compiler will resolve automatically template description :
   * T = void (*)()
   * A = int
   * B = int
   */
  functionArgs(&doItArgs,(int)5,(double)10);

  Base * base = new Base;
  /**
   * C = Base
   */
  function(&Base::doIt, base);


  return 0;
}
