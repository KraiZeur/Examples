

class ThreadingModel<T>
{
public:
  class Lock
  {
    Lock(T* pointee)
  };

};

CheckingPolicy<T>
{
private:
public:
  void Check(T* pointee)
  {

  }

};


template < class T, template <class> class CheckingPolicy, template <class> class ThreadingModel >
class SmartPtr : public CheckingPolicy<T> , public ThreadingModel<SmartPtr>
{
private:
 T* operator->()
 {
   typename ThreadingModel<SmartPtr>::Lock guard(*this);
   CheckingPolicy<T>::Check(pointee_);
   return pointee_;
 }
private:
 T* pointee_;
};
