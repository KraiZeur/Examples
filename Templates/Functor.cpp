#include <iostream>
#include <cstdlib>
#include <cstring>
// http://www.tutok.sk/fastgl/callback.html


float convertIntToFloat(int value) {
    return (float) value;
}


class FunctorBase{
public:
	typedef void (FunctorBase::*_MemFunc)();
	typedef void (*_Func)();
public:
	FunctorBase(): callee(0), func(0) {
	}
	FunctorBase(const void *c,const void *f,size_t sz) {
		if(c) {
			callee = (void *)c;
			memcpy(memFunc,f,sz);
		} else {
			func = f;
		}
	}
	//for evaluation in conditions
	//will be changed to bool when bool exists
	operator int() const{ 
		return func||callee;
	}

	class DummyInit{
	};
////////////////////////////////////////////////////////////////
// Note: this code depends on all ptr-to-mem-funcs being same size
// If that is not the case then make memFunc as large as largest
////////////////////////////////////////////////////////////////
	union {
		const void *func;
		char memFunc[sizeof(_MemFunc)];
	};
	void *callee;
};

template <class P1>
class Functor1: protected FunctorBase{
public:
	Functor1(DummyInit * = 0) {

	}
	void operator()(P1 p1) const {
		std::cout << "Functor1 with arg : " << p1 << std::endl;
		thunk(*this,p1);
	}
public:
	using FunctorBase::operator int;
protected:
	typedef void (*Thunk)(const FunctorBase &,P1);
	Functor1(Thunk t,const void *c,const void *f,size_t sz):
				FunctorBase(c,f,sz),thunk(t) {

	}
private:
	Thunk thunk;
};

// P1 = int | Func = float (*) (int)
template <typename P1, typename Func>
class FunctionTranslator1 : public Functor1<P1> {

private:
public:
    FunctionTranslator1(Func f) : Functor1<P1>(thunk,0,(const void*)f,0){}
	static void thunk(const FunctorBase &ftor,P1 p1) {
		(Func(ftor.func))(p1);
    }
};

// P1 = int | TRT = float | TP1 = int
template <typename P1, typename TRT, typename TP1>
inline FunctionTranslator1<P1, TRT (*)(TP1)> makeFunctor(Functor1<P1>* p1, TRT (*f)(TP1)) {
    return FunctionTranslator1<P1, TRT (*)(TP1)>(f);
}

// prebuild
// setParameter (function, int, int int)
/*  Must generate all template with sized arguments
template <typename A>
void setParameter(std::string name, A a) {
	return Functor1<A>(a);
} */

int main(void) {
    // float convertIntToFloat(int value)
    Functor1<int> f1 = makeFunctor((Functor1<int> *)0,&convertIntToFloat);
    f1(5);

    return 0;
}

