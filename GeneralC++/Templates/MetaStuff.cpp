#include <string>
#include <tuple>

class Person
{
private:
  std::string _name;
public:
  void setName(std::string name)
  {
    _name=name;
  }
  std::string getName()
  {
    return _name;
  }
};

namespace meta
{
  template <typename Class, typename T>
  class Member
  {
  private:
    std::string _name;
    T(Class::*)() const _getterPtr;
    void (Class::*)(const T&) _setterPtr;
  public:
    Member(const char* name, member_ptr_t<Class, T> ptr)
    {

    }
    Member(std::string name, T (Class::*getterPtr)() const, void (Class::*setterPtr)(const T&))
    {
      _getterPtr=getterPtr;
      _setterPtr=setterPtr;;
    }
    virtual ~Member()
    {

    }
    T get()
    {
      // specialized
    }
    void set(T type)
    {
      // specialized
    }
  };
}

namespace meta
{
  template <typename... Args>
  auto members(Args&&... args)
  {
      return std::make_tuple(std::forward<Args>(args)...);
  }

  template <typename Class>
  inline auto registerMembers()
  {
      return std::make_tuple();
  }
}

namespace meta
{

template <>
inline auto registerMembers<Person>()
{
  return members( members("name", &Person::getName, &Person::setName) );
}


template <typename T, typename TupleType>
struct MetaHolder {
    static TupleType members;
    static const char* name() 
    {
        return registerName<T>();
    }
};

template <typename T, typename TupleType>
TupleType MetaHolder<T, TupleType>::members = registerMembers<T>();


template <typename Class>
const auto& getMembers()
{
    return MetaHolder<Class, decltype(registerMembers<Class>())>::members;
}

template <typename Class, typename F, typename>
void doForAllMembers(F&& f)
{
    detail::for_tuple(std::forward<F>(f), getMembers<Class>());
}

template <typename T, typename Class>
T getMemberValue(Class& obj, const char* name)
{
    T value;
    doForMember<Class, T>(name,
        [&value, &obj](const auto& member)
        {
            value = member.get(obj);
        }
    );
    return value;
}

}

using namespace meta;

int main(void) {
  Person person;
  auto name = meta::getMemberValue<std::string>(person, "name");

  //auto mem = getMembers<Person>();


}
