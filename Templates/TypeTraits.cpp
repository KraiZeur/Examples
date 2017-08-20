#include <iostream>

class NullType {};

template <typename T>
class TypeTraits {
private:
 template <class U> struct PointerTraits {
    enum {
        result = false
    };
    //typedef NullType PointeeType;
 };
 template <class U> struct PointerTraits<U*> {
    enum {
        result = true
    };
    //typedef U PointeeType;
 };
public:
 enum {
    isPointer = PointerTraits<T>::result
 };
 //typedef typename PointerTraits<T>::PointeeType PointeeType;
};

template <typename T>
class TypeTraits2 {
private:
 template <class U>
 struct PToMTraits {
    enum {
        result = false
    };
 };
 template <class U, class V>
 struct PToMTraits<U V::*> {
    enum { result = true };
 };
public:
 enum {
    isMemberPointer = PToMTraits<T>::result
 };
 static void display() {}
};

int main(void) {
    int value=50;
    int* ptr;

    ptr=&value;

    bool isPtr = TypeTraits<int*>::isPointer;

    bool isMemberPtr=TypeTraits2<int>::isMemberPointer;

    std::cout << " " << isMemberPtr << std::endl;

    return 0;
}
