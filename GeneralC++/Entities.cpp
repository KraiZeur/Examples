#include <iostream>

class AbstractComponent
{
private:
public:
  AbstractComponent()
  {
  }
  virtual ~AbstractComponent()
  {
  }
};

class GraphicComponent : public AbstractComponent
{
private:
public:
  GraphicComponent()
  {
  }
  virtual ~GraphicComponent()
  {
  }
};

template <typename C>
class Entity
{
private:
  std::map<std::type_index, C*> _components;
public:
  Entity()
  {
  }
  ~Entity()
  {
  }
  template <typename T>
  bool add(C* component)
  {
    std::map<std::type_index, C*>::iterator it;
    it =_components.find(std::type_index(typeid(T)));

    if (it==_components.end())
    {
      _components[std::type_index(typeid(T))]=component;
      return true;
    }

    return false;
  }
  template <typename T>
  T* get()
  {
    std::map<std::type_index, C*>::iterator it;
    it =_components.find(std::type_index(typeid(T)));

    if (it!=_components.end())
    {
      return dynamic_cast<T*>(it->second);
    }

    return NULL;
  }
};

int main(int argc, char const *argv[]) {

  Entity<AbstractComponent> entity;

  entity.add<GraphicComponent>(new GraphicComponent());

  GraphicComponent* graphicComp = entity.get<GraphicComponent>();

  if (graphicComp)
  {

  }

  return 0;
}
