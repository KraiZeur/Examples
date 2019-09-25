#include <map>
#include <string>
#include <typeindex>
#include <iostream>

class AbstractComponent {
private:
public:
    AbstractComponent() {}
    virtual ~AbstractComponent() {}

    virtual void display() const=0;
};

class ComponentA : public AbstractComponent {
private:
public:
    ComponentA() {}
    virtual ~ComponentA() {}

    void display() const {
        std::cout << "I'm ComponentA" << std::endl;
    }
};

class ComponentB : public AbstractComponent {
private:
public:
    ComponentB() {}
    virtual ~ComponentB() {}

    void display() const {
        std::cout << "I'm ComponentB" << std::endl;
    }
};
std::map<std::type_index, AbstractComponent*> _components;

template <typename T>
T* get() {
    auto it = _components.find(std::type_index(typeid(T)));
    if (it != _components.end()) {
        return dynamic_cast<T*>(it->second);
    }
    return nullptr;
}

int main(void) {

    _components[std::type_index(typeid(ComponentB))] = new ComponentB();

    AbstractComponent* comp = get<ComponentB>();

    comp->display();

    return 0;
}
