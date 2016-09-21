#include <pthread.h>
#include <iostream>

/**
 * Resource Acquisition Is Initialization
 */

class Synchro;

class Mutex {
	friend class Synchro;
private:
	pthread_mutex_t _mutex;
public:
	Mutex() {
		pthread_mutex_init(&_mutex, NULL);
	}
	~Mutex() {
		pthread_mutex_destroy(&_mutex);
	}
};

/**
 * Unlock mutex when it run out of scope
 */
class Synchro {
private:
	Mutex * _mutex;
public:
	Synchro(Mutex * mutex) : _mutex(mutex) {
		pthread_mutex_lock(&(_mutex->_mutex));
	}
	~Synchro() {
		pthread_mutex_unlock(&(_mutex->_mutex));
	}
};

Mutex * mutex = new Mutex();
int value=0;

void * routine(void * args) {
	{
		Synchro s(mutex);
		value++;
	}

	pthread_exit(NULL);
}

#define THREADS_COUNT 30000

int main(int argc, char const *argv[]) {

	pthread_t tid[THREADS_COUNT];

	for (int i = 0; i < THREADS_COUNT; i++) {
		pthread_create(tid+i, NULL, routine, NULL);
	}
	for (int i = 0; i < THREADS_COUNT; i++) {
		pthread_join(tid[i], NULL);
	}

	std::cout << value << std::endl;

	return 0;
}