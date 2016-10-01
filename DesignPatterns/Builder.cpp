#include <iostream>
#include <string>
#include <vector>

class Pizza {
private:
  std::vector<std::string> _elements;
public:
  void addElement(std::string elem) {
    _elements.push_back(elem);
  }
  void display() {
    std::vector<std::string>::const_iterator it;
    for(it=_elements.begin();it!=_elements.end();it++) {
      std::cout << (*it) << " ";
    }
     std::cout << std::endl;
  }
};

class Builder {
private:
  Pizza* _pizza;
  std::vector<std::string> _elements;
private:
public:
  Builder() : _pizza(NULL) {}
  Builder* addElement(std::string elem) {
    if(_pizza==NULL)
      _elements.push_back(elem);
      
    return this;
  }
  Pizza* getResult() {
    if (_pizza==NULL) {
      _pizza=new Pizza;
      std::vector<std::string>::const_iterator it;
      for(it=_elements.begin();it!=_elements.end();it++) {
        _pizza->addElement(*it);
      }
    }

    return _pizza;
  }
};

int main(int argc, char const *argv[]) {

  Builder* builder = new Builder;
  builder->addElement("Pasta")->addElement("Mozarella")->addElement("Tomatoes");

  Pizza* pizza=builder->getResult();

  pizza->display();

  return 0;
}
