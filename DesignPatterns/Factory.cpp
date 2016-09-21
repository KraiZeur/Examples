#include <iostream>
#include <typeinfo>
#include <map>
#include <string>

// More pleasant way since typeid is uniq but can't easily create from a string
/*
#define REGISTER_FACTORY(name) \
template<typename B>
static void factoryRegister(TemplateFactory<B>* factory) {
  factory->addType<X>(typeid(X).name());
}
*/
// -----------------------------------------------------------------------------
// use 'template' keyword to treat 'addType' as a dependent template name
// -----------------------------------------------------------------------------
#define REGISTER_FACTORY(name) \
template<typename B> \
static void factoryRegister(TemplateFactory<B>* factory) { \
  factory->template addType<name>(#name); \
}
// -----------------------------------------------------------------------------
// Template factory design
template <typename B, typename T>
B* create() {
  return new T;
}
// -----------------------------------------------------------------------------
template <typename B>
class TemplateFactory {
private:
  std::map<std::string, B* (*)(void)> _creators;
public:
  TemplateFactory();
  ~TemplateFactory() {}
  template <typename T>
  bool addType(std::string component) {
    typedef typename std::map<std::string, B* (*)(void)>::const_iterator iter;
    iter it;
    it=_creators.find(component);
    if (it==_creators.end()) {
      B* (*ptr)(void) = &::create<B,T>;
      _creators[component] = ptr;
      return true;
    }
    std::cerr << "Can't register " << component <<
            " already registered with the same name" << std::endl;
    return false;
  }
  B* create(std::string component) {
    typedef typename std::map<std::string, B* (*)(void)>::const_iterator iter;
    iter it;
    it=_creators.find(component);
    if (it!=_creators.end()) {
      return it->second();
    }
    return NULL;
  }
};
// -----------------------------------------------------------------------------
class Base {
private:
public:
  virtual ~Base() {}
  virtual void display()=0;
};
// -----------------------------------------------------------------------------
class ConcreteA : public Base {
private:
public:
  virtual ~ConcreteA() {}
  void display() const {
    std::cout << "I'm ConcreteA" << std::endl;
  }
  REGISTER_FACTORY(ConcreteA)
};
// -----------------------------------------------------------------------------
class ConcreteB : public Base {
private:
public:
  virtual ~ConcreteB() {}
  void display() {
    std::cout << "I'm ConcreteB" << std::endl;
  }
  REGISTER_FACTORY(ConcreteB)
  //friend void TemplateFactory<B>::addType(std::string component);
};
// -----------------------------------------------------------------------------
// Default factory design
class Factory {
private:
public:
  static Base* createConcrete(std::string id) {
    if (id=="ConcreteA")
      return new ConcreteA;
    else if (id=="ConcreteB")
      return new ConcreteB;
    return NULL;
  }
};
// -----------------------------------------------------------------------------
template<typename B>
TemplateFactory<B>::TemplateFactory() {
  ConcreteA::factoryRegister<B>(this);
  ConcreteB::factoryRegister<B>(this);
}

// -----------------------------------------------------------------------------
// Base* base=Factory::createConcrete();
// -----------------------------------------------------------------------------
int main(void) {
  TemplateFactory<Base>* factory = new TemplateFactory<Base>();

  Base* concrete=factory->create("ConcreteA");
  if (concrete) {
    concrete->display();
  } else {
    std::cerr << "Error no factory defined for class " << "ConcreteA" << std::endl;
  }

  return 0;
}
