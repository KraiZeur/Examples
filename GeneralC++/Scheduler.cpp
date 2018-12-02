// https://github.com/bitcoin/bitcoin/blob/master/src/scheduler.cpp

#include <map>

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <sys/time.h>

#define THREAD_DEFAULT_PRIO 5//DEFAULT_PRIO
#define THREAD_MAX_PRIO 99//MAX_PRIO-1
#define THREAD_MIN_PRIO 1

class Lock;
class Monitor ;

class Mutex {
	friend class Lock;
	friend class Monitor;
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

class Monitor {
private:
  pthread_cond_t _cv;
public:
  Monitor()
  {
	  pthread_cond_init(&_cv, NULL);
  }
  ~Monitor()
  {
	  pthread_cond_destroy(&_cv);
  }
  void wait(Mutex* mutex)
  {
	  {
		Lock lock(mutex);

		pthread_cond_wait(&_cv, &(mutex->_mutex));
	  }
  }
  int timed_wait(Mutex* mutex, int absolute_time)
  {
	int rc=0;

	struct timespec ts;
	ts.tv_sec = absolute_time;
	ts.tv_nsec = 0;

	{
		Lock lock(mutex);

		rc=pthread_cond_timedwait(&_cv, &(mutex->_mutex), &ts);
	}

	return rc;
  }
  void notify(Mutex* mutex)
  {
	{
		Lock lock(mutex);

		pthread_cond_signal(&_cv);
	}
  }

};

class Runnable
{
public:
	inline virtual void pre_run()
	{
		this->run();
	}
	virtual void run()=0;
};

//class Thread;

static void* __Thread_Entry_Point(void* args)
{
	Runnable* runnable = (Runnable*) args;
	runnable->pre_run();
}

#define THV_EMPTY (0)
#define THV_INIT_DONE (1 << 1)
#define THV_RUNNING (1 << 2)
#define THV_KILL (1 << 4)

class Thread
{
private:
	pthread_t _tid;
	pthread_attr_t _tattr;
	Runnable* _runnable;
	int _priority;

	uint16_t _flags;
public:
	Thread(Runnable* runnable)
	{
		_flags=THV_EMPTY;
		_runnable=runnable;
		_priority=THREAD_DEFAULT_PRIO;
	}
	void setPriority(int priority)
	{
		if (priority <= THREAD_MAX_PRIO && priority >= THREAD_MIN_PRIO)
		{
			_priority=priority;
		}
		else
		{
			std::cerr << "Error priority not in bound" << std::endl;
		}
	}
	void start()
	{
		struct sched_param sched;

		int ret=pthread_attr_init (&_tattr);
		ret=pthread_attr_getschedparam (&_tattr, &sched);
		std::cout << "pthread_attr_getschedparam " << ret << std::endl;

		ret=pthread_attr_setschedpolicy(&_tattr, SCHED_FIFO);
		std::cout << "pthread_attr_setschedpolicy " << ret << std::endl;

		sched.sched_priority=_priority;
		ret=pthread_attr_setschedparam (&_tattr, &sched);
		std::cout << "pthread_attr_setschedparam " << ret << std::endl;

		ret=pthread_create(&_tid, &_tattr, __Thread_Entry_Point, (void*) _runnable);
		std::cout << "pthread_create " << ret << std::endl;
	}
	void join()
	{
		pthread_join(_tid, NULL);
	}
	void waitForFlag(uint16_t flag)
	{
		while (!(_flags & flag))
		{
			usleep(100);
		}
		return;
	}
};
// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

template <typename R>
class Scheduler : public Thread, public Runnable
{
private:
	Mutex* _mutex;
	Monitor* _monitor;
	
	bool _run;
	std::multimap<int, R> _taskQueue;
private:
	template <class U> struct PointerTraits
	{
		enum { result = false };

		static void evaluate(U task)
		{
			task();
		}
	};
	template <class U> struct PointerTraits<U*>
	{
		enum { result = true }; 

		static void evaluate(U* task)
		{
			(*task)();
		}
	};
protected:
public:
	Scheduler() : Thread(this)
	{
		_run=true;
		_mutex=new Mutex();
		_monitor=new Monitor();
	}
	~Scheduler()
	{
		delete _mutex;
		delete _monitor;
	}

	bool schedule(R task, int timeout)
	{
		{
			Lock lock(_mutex);
			_taskQueue.insert(std::make_pair(timeout, task));
		}

		_monitor->notify(_mutex);
	}

	bool repeat(R task, int timeout)
	{
		PointerTraits<R>::evaluate(task);
		schedule(task, timeout);

		// schedule( FunctionPointer2<bool, R, int>(this, &Scheduler::repeat) )
	}

	void run()
	{
		while (shouldRun())
		{
			if (shouldRun() && _taskQueue.empty())
			{
				_monitor->wait(_mutex);
			}

			/* _monitor->wait(_mutex) return 110 if it's unlocked by the timer */
			/* we need a loop to be sure that the wait is unlocked by the timer and not an added task */
			while (shouldRun() && !_taskQueue.empty() && (_monitor->timed_wait(_mutex, _taskQueue.begin()->first) != 110 ))
			{ }

			PointerTraits<R>::evaluate(_taskQueue.begin()->second);

			_taskQueue.erase(_taskQueue.begin());
		}
	}

	bool shouldRun() const
	{
		return _run;
	}
};

class Task
{
private:
	int _param;
public:
	Task(int param)
	{
		_param=param;
	}
	void operator()()
	{
		std::cout << "Hello " << _param << std::endl;
	}
};

int main(int argc, char const *argv[])
{
	Scheduler<Task*>* _scheduler = new Scheduler<Task*>();
	_scheduler->start();

//    _thread->waitFlag(THV_INIT_DONE);

	struct timeval now;
	gettimeofday(&now,NULL);

	int time1=now.tv_sec+10;

	_scheduler->schedule(new Task(1), time1);

	sleep(3);

	gettimeofday(&now,NULL);
	int time2=now.tv_sec+5;

	_scheduler->schedule(new Task(2), time2);

	char buffer[512];

	while(fgets(buffer, 512, stdin))
	{
		std::cout << "Buffer" << buffer << std::endl;
		int time_off=atoi(buffer);

		gettimeofday(&now,NULL);	
		time1=now.tv_sec+time_off;

		_scheduler->schedule(new Task(time_off), time1);
	}

	return 0;
}

/*

@TODO ::

+ Scheduling by time not working

+ ThreadVar

*/