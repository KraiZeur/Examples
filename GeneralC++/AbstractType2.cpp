#include <iostream>
#include <cstring>

#define DEFINE_VISITABLE() \
 virtual ReturnType accept(BaseVisitor& visitor) \
 { return AcceptImpl(*this, visitor); }

class BaseVisitor {
public:
  virtual ~BaseVisitor() {}
};

template <typename T, typename R  = void>
class Visitor {
private:
public:
  virtual R visit(T& visitable) = 0;
};

template <typename R = void>
class BaseVisitable {
public:
  typedef R ReturnType;
private:
protected:
  template <class T>
  ReturnType AcceptImpl(T& visited, BaseVisitor& guest) {
    Visitor<T, R>* p = dynamic_cast<Visitor<T, R>*>(&guest);
    if(p) {
      return p->visit(visited);
    }
    return ReturnType();
  }
public:
  virtual ~BaseVisitable() {}
  virtual R accept(BaseVisitor& visitor) = 0;
};

class AbstractTMObserver {
private:
public:
  virtual void printValue() const = 0;
};


class AbstractType : public BaseVisitable<> {
private:
protected:
  void* _data;
public:
  AbstractType() {
    _data=NULL;
  }
  virtual ~AbstractType() {
  }
  DEFINE_VISITABLE()
};

template <typename T>
class VType : public AbstractType {
private:
public:
  VType(const T* data) : AbstractType() {
    _data=new T;
    memcpy(_data, data, sizeof(T));
  }
  virtual ~VType() {
    if(_data) {
      T* data = reinterpret_cast<T*>(_data);
      delete data;
      _data=NULL;
    }
  }
  T getValue() {
    return *(reinterpret_cast<int*>(_data));
  }
  DEFINE_VISITABLE()
};

typedef VType<int> Int;
typedef VType<float> Float;
typedef VType<double> Double;


template <typename T>
class TMObserver : public AbstractTMObserver {
private:
  VType<T>* _data;
public:
  TMObserver() {
    _data=NULL;
  }
  TMObserver(VType<T>* data) {
    _data=data;
  }
  virtual ~TMObserver() {

  }
  void printValue() const {
    std::cout << "Value is : " << _data->getValue() << std::endl;
  }
};

template<class T>
using VisitorType = Visitor<VType<T> >;

/**
 * Allows this factory to create TMObserver based on Int VType
 */
class TMFactoryDynamic : public BaseVisitor, public VisitorType<int> {
private:
  AbstractTMObserver* _data;
public:
  TMFactoryDynamic() {
    _data=NULL;
  }
  AbstractTMObserver* generateFrom(AbstractType* data) {
    data->accept(*this);
    AbstractTMObserver* ret = _data;
    _data=NULL;
    return ret;
  }

  void visit(VType<int>& data) {
    _data=new TMObserver<int>(&data);
  }
};

int main(int argc, char const *argv[]) {
  int value=150;
  AbstractType* data = new Int(&value);

  TMFactoryDynamic factory;
  AbstractTMObserver* observer = factory.generateFrom(data);
  observer->printValue();

  return 0;
}
