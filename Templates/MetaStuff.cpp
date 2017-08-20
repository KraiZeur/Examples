#include <string>


class Person
{
private:
public:
  void setName(std::string name)
  {
    _name=name;
  }
  std::string name getName()
  {
    return _name;
  }
};


namespace meta
{
  template <typename T, typename TupleType>
  struct MetaHolder {
    static TupleType members;
  };

  template <typename T, typename TupleType>
  TupleType MetaHolder<T, TupleType>::members = registerMembers<T>();
}

namespace meta
{
  template <typename C>
  const auto& getMembers()
  {
      return detail::MetaHolder<C, decltype(registerMembers<C>())>::members;
  }
}

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


// #############################################################################
namespace meta
{
  template <>
  inline Member registerMembers<Person>()
  {
    return Member( Member("Name", &Person::getName, &Person::setName) );
  }
}
// #############################################################################


int main(int argc, char const *argv[]) {

  //MetaHolder<Person, >::members;

  meta::getMembers<Person>();

  // Template<T, C>
  // T = Person
  // C = String
  meta::setMemberValue<std::string>(person, "name", "Ron Burgundy");

  return 0;
}
