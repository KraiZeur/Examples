#include <iostream>
#include <map>
#include <unistd.h>
#include <pthread.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void* routine(void* args)
{
    sleep(1);

    std::map<int, int> *values = (std::map<int, int> *) args;

    std::map<int, int>::iterator it;

    pthread_rwlock_wrlock(&rwlock);
    for (it = values->begin(); it != values->end(); it++)
    {
        std::cout << "Removing " << it->first << " " << it->second << std::endl;

        it=values->erase(it);
    }
    pthread_rwlock_unlock(&rwlock);
}

int main(int argc, char const *argv[])
{
    std::map<int, int> values;

    values[0]=1;
    values[1]=2;
    values[2]=3;

    std::map<int, int>::iterator it;
    for (it = values.begin(); it != values.end(); it++)
    {
        std::cout << it->first << " " << it->second << std::endl;
    }

    pthread_t tid;
    int ret=pthread_create(&tid, NULL, routine, &values);

    bool start=true;

    pthread_rwlock_rdlock(&rwlock);
    for (it = values.begin(); it != values.end(); it++)
    {
        if (start)
            sleep(5);

        start=false;

        std::cout << it->first << " " << it->second << std::endl;
    }
    pthread_rwlock_unlock(&rwlock);

    sleep(15);

    return 0;
}
