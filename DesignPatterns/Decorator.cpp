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
  Decorator(Component* component) : _component(component) {}
  virtual ~Decorator() {
    delete _component;
  }
};

class Mozarella : public Decorator {
private:
public:
  Mozarella(Component* component) : Decorator(component) { }
  virtual ~Mozarella() {}
  unsigned int getPrice() {
    return 75 + _component->getPrice();
  }
};

int main(int argc, char const *argv[]) {
  Component* component = new Mozarella(new Pizza);

  std::cout << "Final price : "  << component->getPrice() << std::endl;

  delete component;

  return 0;
}
