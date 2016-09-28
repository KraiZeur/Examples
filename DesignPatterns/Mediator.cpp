#include <iostream>
#include <vector>

/**
 * Mediator allows synchronisation between actors in contrary to observer pattern
 * (order not known)
 */

class Mediator;

class UserInterface {
public:
  virtual void setMediator(Mediator* mediator)=0;
  virtual void sendMessage(std::string message)=0;
  virtual void receiveMessage(std::string message)=0;
};

class Mediator {
private:
  UserInterface* _u1;
  UserInterface* _u2;
public:
  Mediator(UserInterface* u1, UserInterface* u2) : _u1(u1), _u2(u2) {
    _u1->setMediator(this);
    _u2->setMediator(this);
  }
  virtual ~Mediator() {}
  void send(UserInterface* sender, std::string message) {
    if (sender==_u1) {
      _u2->receiveMessage(message);
    } else {
      _u1->receiveMessage(message);
    }
  }
};

class User : public UserInterface {
private:
  Mediator* _mediator;
  std::string _name;
public:
  User(std::string name) : _name(name) {}
  virtual ~User() {}
  void setMediator(Mediator* mediator) {
    _mediator = mediator;
  }
  virtual void sendMessage(std::string message) {
    _mediator->send(this,message);
  }
  virtual void receiveMessage(std::string message) {
    std::cout << "Received : " << message << " received from " << _name << std::endl;
  }
};

int main(int argc, char const *argv[]) {
  User* jean = new User("Jean");
  User* michel = new User("Michel");

  Mediator* mediator = new Mediator(jean,michel);

  jean->sendMessage("Salut c'est jean");

  delete jean;
  delete michel;

  return 0;
}
