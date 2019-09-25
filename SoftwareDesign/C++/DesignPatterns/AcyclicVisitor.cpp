#include <iostream>
#include <vector>

/**
 *  http://stackoverflow.com/questions/8919566/are-pure-virtual-methods-allowed-within-a-template-class
 */

 class BaseVisitor {
 public:
   virtual ~BaseVisitor() {}
 };

 template<typename T>
 class Visitor {
 private:
 public:
   virtual void visit(T& visitable) = 0;
 };

 #define DEFINE_VISITABLE() \
  virtual void accept(BaseVisitor& visitor) \
  { return AcceptImpl(*this, visitor); }

class BaseVisitable {
private:
public:
  virtual ~BaseVisitable() {}
  virtual void accept(BaseVisitor& visitor) = 0;
protected:
  template <class T>
  static void AcceptImpl(T& visited, BaseVisitor& guest) {
    if (Visitor<T>* p = dynamic_cast<Visitor<T>*>(&guest)) {
      p->visit(visited);
    }
  }
};

class Fruit : public BaseVisitable {
private:
public:
  virtual ~Fruit() {}
  DEFINE_VISITABLE()
};

class Apple : public Fruit {
private:
public:
  virtual ~Apple() {}
  DEFINE_VISITABLE()
};

class Orange : public Fruit {
private:
public:
  virtual ~Orange() {}
  DEFINE_VISITABLE()
};

/**
 * BaseVisitable will try to upCast to Visitor<Apple> when the object is an
 * Apple.
 * And upCast to Visitor<Orange> When the object is an Orange
 * Since this class doesn't inherit from Visitor<Orange>
 * Nothing will happen
 */
class AppleCounter : public BaseVisitor, public Visitor<Apple> {
private:
  unsigned int _counter;
  std::vector<Apple*> _apples;
public:
  AppleCounter() {
    _counter=0;
  }
  void visit(Apple& apple) {
    _counter++;
    _apples.push_back(&apple);
  }
  int getCount() {
    for(std::vector<Apple*>::iterator it=_apples.begin(); it!=_apples.end(); it++) {
      std::cout << "Apple " << &(*it) << std::endl;
    }
    return _counter;
  }
};

int main(int argc, char const *argv[]) {
  AppleCounter counter;

  std::vector<Fruit*> _fruits;
  _fruits.push_back(new Apple);
  _fruits.push_back(new Apple);
  _fruits.push_back(new Orange);
  _fruits.push_back(new Apple);

  for(std::vector<Fruit*>::const_iterator it=_fruits.begin(); it!=_fruits.end(); it++) {
    (*it)->accept(counter);
  }

  std::cout << "Apple Count : "<< counter.getCount() << std::endl;

  return 0;
}
