#include <iostream>

#ifdef IN_PROGRESS
typedef enum transition {
	DEFINED,
	INIT,
	START,
	STOP
} transition_t;

template <typename T>
class StateMachine {


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

/*
	Maybe create inbetween controller like :

									 STOPPED
									 	|
									 	|
									  INIT
										|
									[Controller]
										|
					    [Controller]    |
					    	  v			v
	DEFINED ------- INIT --------> INITIALIZED -> [Cabllback]

	On State Changed to INIT call Callback
	On transition INIT from either DEF or STO call specific Controller if No Controller go to Callback

*/
class MyCls : public StateMachine<transition_t> {
private:
	std::map < 'currentState' , std::map<transition_t, void (C::*)(transition_t)> > _states;
public:
	MyCls() {
		//registerState(defined, INIT, initiated, (function));
	}
	void registerState(int transition,  void (C::*callback)(transition_t)) {

	}
	bool onStateInit(transition_t transition) {

	}
	bool onStateStart(transition_t transition) {

	}
	bool onStateEnd(transition_t transition) {

	}
	bool step(transition_t transition) {
		auto it = _states.find(transition);
		if (it!=_states.end()) {
			auto func = it->second;
			return _owner->func(transition);
		}
		return false;
	}
};

class OtherWay {
private:
public:
}

int main(int argc, char const* argv[]) {
	MyCls* cls = new MyCls;

	cls->step(/*INIT*/);
	cls->step(/*START*/);
	cls->step(/*STOP*/);

	return 0;
}
#endif /*IN_PROGRESS*/

int main(int argc, char const *argv[]) {

	return 0;
}
