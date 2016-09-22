#include <iostream>
#include <vector>

template <typename T>
class Observer {
private:
public:
	virtual void observableChanged(T * ptr)=0;
};

template <typename T>
class Observable {
protected:
	std::vector<Observer<T> *> _observers;
public:
	void addObserver(Observer<T> * observer) {
		_observers.push_back(observer);
	}
	virtual void notifyObservers(T * observable) {
		typename std::vector<Observer<T> *>::const_iterator it;
		for(it=_observers.begin(); it!=_observers.end(); it++) {
			(*it)->observableChanged(observable);
		}
	}
};

class Television : public Observable<Television> {
private:
	int _channel;
public:
	void setChannel(int channel) {
		_channel=channel;
		notifyObservers(this);
	}
	void printValue() {
		std::cout << "New TV Channel is : " << _channel << std::endl;
	}
};

class TestObserver : public Observer<Television> {
private:
public:
	virtual void observableChanged(Television * tv) {
		tv->printValue();
	}
};

int main(int argc, char const *argv[]) {

	TestObserver * observer = new TestObserver();

	Television * tv = new Television();
	tv->addObserver(observer);

	tv->setChannel(17);

	return 0;
}
