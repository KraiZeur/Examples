#include <iostream>

class Base {
private:
public:
	Base() {

	}
	virtual ~Base() {

	}
	virtual void print() {

	}
};

class ConcreteA : public Base {
private:
public:
	ConcreteA() : Base() {
		
	}
	virtual ~ConcreteA() {
		
	}
	virtual void print() {
		std::cout << std::endl;
	}
};

int main(int argc, char const *argv[]) {

	Base* base = new ConcreteA;
	
	delete base;

	return 0;
}