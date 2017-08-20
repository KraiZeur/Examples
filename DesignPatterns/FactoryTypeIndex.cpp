#include <iostream>
#include <typeinfo>
#include <map>
#include <string>
#include <typeindex>

// -----------------------------------------------------------------------------
// use 'template' keyword to treat 'addType' as a dependent template name
// -----------------------------------------------------------------------------
#define FACTORY_IMPL(cls) \
template<typename B> \
static void factoryRegister(TemplateFactory<B>* factory) { \
  factory->template addType<cls>(); \
}
// -----------------------------------------------------------------------------
#define REGISTER_FACTORY(cls) \
cls::factoryRegister<B>(this);
// -----------------------------------------------------------------------------
template <typename B>
class TemplateFactory {
private:
  std::map<std::type_index, B* (*)(void)> _creators;
private:
  template <typename T>
  static B* static_create() {
    return new T;
  }
public:
  TemplateFactory();
  ~TemplateFactory() {}
  template <typename T>
  bool addType() {
    typedef typename std::map<std::type_index, B* (*)(void)>::const_iterator iter;
    iter it;
    it=_creators.find(std::type_index(typeid(T)));
    if (it==_creators.end()) {
      B* (*ptr)(void) = &TemplateFactory::static_create<T>;
      _creators[std::type_index(typeid(T))] = ptr;
      return true;
    }
    std::cerr << "Can't register " << typeid(T).name() <<
            " already registered with the same name" << std::endl;
    return false;
  }
  template <typename T>
  B* create() {
    typedef typename std::map<std::type_index, B* (*)(void)>::const_iterator iter;
    iter it;
    it=_creators.find(std::type_index(typeid(T)));
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
  virtual void display() const=0;
};
// -----------------------------------------------------------------------------
class ConcreteA : public Base {
private:
public:
  virtual ~ConcreteA() {}
  virtual void display() const {
    std::cout << "I'm ConcreteA - " << typeid(ConcreteA).name() << std::endl;
  }
  FACTORY_IMPL(ConcreteA)
};
// -----------------------------------------------------------------------------
class ConcreteB : public Base {
private:
public:
  virtual ~ConcreteB() {}
  virtual void display() const {
    std::cout << "I'm ConcreteB - " << typeid(ConcreteB).name() << std::endl;
  }
  FACTORY_IMPL(ConcreteB)
  //friend void TemplateFactory<B>::addType(std::string component);
};

// -----------------------------------------------------------------------------
template<typename B>
TemplateFactory<B>::TemplateFactory() {
  REGISTER_FACTORY(ConcreteA)
  REGISTER_FACTORY(ConcreteB)
}

// -----------------------------------------------------------------------------
// Base* base=Factory::createConcrete();
// -----------------------------------------------------------------------------
int main(void) {
  TemplateFactory<Base>* factory = new TemplateFactory<Base>();

  Base* concrete=factory->create<ConcreteA>();
  if (concrete) {
    concrete->display();
  } else {
    std::cerr << "Error no factory defined for class " << "ConcreteA" << std::endl;
  }

  delete factory;
  delete concrete;

  return 0;
}
