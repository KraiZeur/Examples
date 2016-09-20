#include <iostream>
#include <string>

/**
 * Simple Chain
 * using an iterator to visit each element.
 */
class Chain {
private:
	Chain* _next;
	std::string _name;
public:
	class Iterator {
	private:
		Chain * _current;
	public:
		Iterator(Chain * current) {
		}
		Iterator operator++(int) {
      _current=_current->next();
      return (*this);
    }
		bool operator==(const Iterator& other) {
  		return ((*this) == (*other));
		}

		bool operator!=(const Iterator& other) {
  		return !(*this == other);
		}
		Chain* operator*() {
			return _current;
		}
	};

	Iterator begin() {
		Iterator it(this);
		return it;
	}

	Iterator end() {
		Iterator it(NULL);
		return it;
	}
public:
	Chain(std::string name) {
		_next=NULL;
		_name=name;
	}

	Chain* next() {
		return _next;
	}

	void setNext(Chain* next) {
		_next=next;
	}

	void displayAll() {
		displayThis();
		if(_next!=NULL)
			_next->displayAll();
	}

	void displayThis() {
		std::cout << _name << std::endl;
	}
};

int main(void) {
	Chain* chain = new Chain("one");
	chain->setNext(new Chain("two"));

	for(Chain::Iterator it=chain->begin();it!=chain->end();it++) {
		(*it)->displayThis();
	}
	return 0;
}
