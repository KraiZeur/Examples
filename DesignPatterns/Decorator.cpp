class Component {
private:
public:
  Component() {}
  virtual ~Component() {}
  virtual unsigned int getPrice()=0;
};

class Pizza : public Component {
private:
public:
  unsigned int getPrice() {
    return 150;
  }
};

class Decorator : public Component {
protected:
  Component* _component;
public:
  virtual ~Decorator() {
    delete _component;
  }
  Decorator(Component* component) {
    _component=component;
  }
};

class Mozarella : public Component {
private:
public:
  virtual ~Mozarella() {}
  unsigned int getPrice() {
    return 75 + _component->getPrice();
  }
};

int main(int argc, char const *argv[]) {
  Component* component = new Mozarella(new Pizza);

  delete component;
  
  return 0;
}
