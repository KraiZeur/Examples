#include <iostream>

namespace singleton {
	class Singleton {
	private:
		static Singleton * _instance;
	private:
		Singleton() {}
	public:
		inline static Singleton * getInstance() {
			if(_instance==NULL)
				_instance=new Singleton;
			return _instance;
		}
		~Singleton() {}
		void display() {
			std::cout << "Hello World" << std::endl;
		}
	};
}

namespace singleton {
	Singleton * Singleton::_instance=NULL;
}

int main(void) {
	singleton::Singleton * instance=singleton::Singleton::getInstance();
	if (instance!=NULL) {
		instance->display();
	}

	return 0;
}
