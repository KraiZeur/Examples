#include <iostream>

template <typename T>
class smart_pointer {
private:
	T* _pointer;
	int _counter;
public:
	smart_pointer(T* pointer) : _pointer(pointer), _counter(1) {

	}
	smart_pointer(const smart_pointer<T>& other) {
		_pointer=other->_pointer;
		_counter=other->_counter;
		_counter++;
	}
	~smart_pointer() {
		_counter--;
		if (_counter==0) {
			delete _pointer;
		}
	}

	T* operator*() {
		return _pointer;
	}
}