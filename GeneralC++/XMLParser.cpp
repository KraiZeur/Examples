#include <iostream>
#include <string>
#include <vector>

class MsgComponent {
private:
  std::string _name;
  std::vector<MsgComponent*> _childs;
public:
  MsgComponent(std::string name) : _name(name)
  {

  }
  // ---------------------------------------------------------------------------
  ~MsgComponent()
  {
    for(std::vector<MsgComponent*>::iterator it=_childs.begin();
        it!=_childs.end; it++)
    {
      delete (*it);
    }
  }
  // ---------------------------------------------------------------------------
  void add(MsgComponent* msg)
  {
    _childs.push_back(msg);
  }
  // ---------------------------------------------------------------------------
  std::string getName() const
  {
    return _name;
  }
  // ---------------------------------------------------------------------------
  MsgComponent* find(std::string name)
  {
    for(std::vector<MsgComponent*>::iterator it=_childs.begin();
        it!=_childs.end; it++)
    {
      if ((*it)->getName() == name)
      {
        return (*it);
      }
    }
    return NULL;
  }
};

template <typename T>
class MessageValue : public MsgComponent {
private:
public:
  MessageValue(std::string name) : MsgComponent(name)
  {

  }
  // ---------------------------------------------------------------------------
  ~MessageValue()
  {
  }
};

namespace test
{
  class Factory
  {
  private:
    template <typename T>
    static MsgComponent* generate(std::string name)
    {
      return new T(name);
    }
  public:
    template <typename T>
    bool registerField(std::string field)
    {
      _map[fieldName] = &::generate<T>;
    }
  };
  /*
    factory->registerField<MessageValue<int> >("data");
  */
  // ---------------------------------------------------------------------------
  MsgComponent* parse(std::string xmlPath)
  {
    /*
    while(hasNext())
    {
        factory->set(fieldName, fieldValue);
    }

    */
    return NULL;
  }
}


/*
Example :

<data>
  <size>1</size>
  <type>int</int>
  <value>75</value>
</data>
*/

// TODO rename MsgComponent by XmlNode

int main(int argc, char const *argv[]) {

  MsgComponent* msg = new MsgComponent("data");
  msg->add(new MessageValue<int>("size"));
  msg->add(new MessageValue<std::string>("type"));
  msg->add(new MessageValue<int>("value"));

  msg->find("size");

  factory->registerField("type", &::registerField<int>);

  /*
    File* file = File::open("data.xml");

    XmlElement* element = file.parse();



  */


  return 0;
}
