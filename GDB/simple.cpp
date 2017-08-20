#include <iostream>

class AbstractA {
private:
public:
	AbstractA() {}
	virtual ~AbstractA() {}
	virtual void basicVirtual() {
	}
	virtual void pureVirtual() = 0;
	void notVirtualRedefined() {}
	void notVirtual() {}
};

class ConcreteA : public AbstractA {
private:
public:
	ConcreteA() {}
	virtual ~ConcreteA() {}
	virtual void basicVirtual() {
	}
	virtual void pureVirtual() {

	}
	void notVirtualRedefined() {}
};

int main(int argc, char const *argv[]) {

	AbstractA* cls = new ConcreteA();

	cls->pureVirtual();

	delete cls;


	return 0;
}