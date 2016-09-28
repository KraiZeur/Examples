#include <iostream>
#include <string>
#include <vector>
#include <map>

class CharacterState {
private:
  int _xp;
  int _life;
public:
  CharacterState(int xp, int life) : _xp(xp), _life(life) {}
  virtual ~CharacterState() {}

  int getXp() {return _xp;}
  int getLife() {return _life;}
};

class Character {
private:
  int _xp;
  int _life;
public:
  Character() : _xp(0), _life(0) {}
  virtual ~Character() {}
  /* Memento specific load & save*/
  void loadState(CharacterState* state) {
    _xp=state->getXp();
    _life=state->getLife();
  }
  CharacterState* saveState() {
    return new CharacterState(_xp,_life);
  }

  void setLife(int life) {_life=life;}
  void setXp(int xp) {_xp=xp;}

  void display() {
    std::cout << "xp : " << _xp << " | life : " << _life << std::endl;
  }
};

class CareTaker {
private:
  std::map<std::string,CharacterState*> _states;
public:
  void addMemento(std::string name,CharacterState* state) {
    _states[name]=state;
  }
  CharacterState* getMemento(std::string name) {
    return _states[name];
  }
};

int main(int argc, char const *argv[]) {

  Character* hellBoy = new Character;
  hellBoy->setLife(150);
  hellBoy->display();

  CareTaker* taker=new CareTaker;
  taker->addMemento("state1", hellBoy->saveState());

  hellBoy->setLife(25);
  hellBoy->display();

  hellBoy->loadState(taker->getMemento("state1"));
  hellBoy->display();

  return 0;
}
