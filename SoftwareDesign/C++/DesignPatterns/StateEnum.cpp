#include <iostream>
#include <map>
#ifdef IN_PROGRESS
typedef enum State {
	DEFINED,
	INITIATED,
	STARTED,
	STOPPED
} state_t;

typedef enum Transition {
	INIT,
	START,
	STOP
} transition_t;

template <typename ST, typename TR>
class StateMachine {
private:
	std::map<ST, std::map<TR,ST> > _states;
	ST _currentState;
public:
	bool registerTransition(ST fromState, TR transition, ST toState) {
		std::map<ST, std::map<TR,ST> >::iterator it=_states.find(fromState);
		if(it==_states.end()) { // fromState Not found
			std::map<TR,ST> mp;
			_states[fromState] = mp;

			mp[transition] = toState; // Added
			return true;
		} else {
			std::map<TR,ST> mp = it->second;

			std::map<TR,ST>::iterator it2=mp.find(transition);

			if (it2==mp.end()) {
				mp[transition]=toState;
				return true;
			}
		}
		return false;
	}
	bool canTransition(TR transition) {
		std::map<ST, std::map<TR,ST> >::iterator it=_states.find(_currentState);
		if(it!=_states.end()) {
			std::map<TR,ST>::iterator it2=it->second.find(transition);
			if(it2!=_states.end()) {
				return true;
			}
		}
		return false;
	}
	bool runTransition(TR transition) {
		std::map<ST, std::map<TR,ST> >::iterator it=_states.find(_currentState);
		if(it!=_states.end()) {
			std::map<TR,ST>::iterator it2=it->second.find(transition);
			if(it2!=_states.end()) {
				_currentState=it2->second;
				return true;
			}
		}
		return false;
	}
};

/**
    -----------              -------------
	| DEFINED | --> INIT --> | INITIATED |
    -----------              -------------
								  |
								  |
								  v
								START
								  |
								  |
								  v
	----------- 		     -----------
	| STOPPED | <-- STOP <-- | STARTED |
	----------- 			 -----------
*/


int main(int argc, char const *argv[]) {

	StateMachine<state_t, transition_t>* stMachine = new StateMachine<state_t, transition_t>();

	stMachine->registerTransition(state_t::DEFINED, transition_t::INIT, state_t::INITIATED);

	delete stMachine;

	return 0;
}

#endif /* IN_PROGRESS */

int main() {
	return 0;
}
