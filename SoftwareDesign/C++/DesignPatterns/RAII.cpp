#include <pthread.h>
#include <iostream>

/**
 * Resource Acquisition Is Initialization.
 * This pattern can be easily presented using mutex mechanism.
 * Since mutex need to be locked and unlocked RAII allow to do this routine
 * witout explicitly calling luck/unlock functions.
 * Moreover when the class goes out of scope it always unlock ressources.
 */

class Lock;

class Mutex {
	friend class Lock;
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
 * Unlock mutex when the class goes out of scope
 */
class Lock {
private:
	Mutex * _mutex;
public:
	Lock(Mutex * mutex) : _mutex(mutex) {
		pthread_mutex_lock(&(_mutex->_mutex));
	}
	~Lock() {
		pthread_mutex_unlock(&(_mutex->_mutex));
	}
};

Mutex * mutex = new Mutex();
int value=0;

void * routine(void * args) {
	{
		Lock lock(mutex); // <-- Here we initialize the lock (lock the mutex) by calling Lock constructor
		value++;
	} // <-- Here the lock goes out of scope (unlock the mutex) by calling Lock destructor

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
