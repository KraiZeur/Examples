#include <iostream>
#include <map>
#include <unistd.h>
#include <pthread.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
bool start=false;

void* routine(void* args)
{
    std::map<int, int> *values = (std::map<int, int> *) args;

    std::map<int, int>::iterator it;

    pthread_mutex_lock(&lock);
    while (!start)
        pthread_cond_wait(&cv, &lock);
    pthread_mutex_unlock(&lock);

    pthread_rwlock_wrlock(&rwlock);

    while (it != values->end())
    {
        std::cout << "Removing " << it->first << " " << it->second << std::endl;

        it = values->erase(it);
    }

    pthread_rwlock_unlock(&rwlock);
}

int main(int argc, char const *argv[])
{
    std::map<int, int> values;

    int element_count=0;
    if (argc < 1)
    {
        fprintf(stderr, "usage : %s <vector count>\n", argv[0]);
        return -1;
    }
    else
    {
        element_count=atoi(argv[1]);

        if (element_count <= 0)
        {
            fprintf(stderr, "error can't process negative/zero values : %d value\n", element_count);
            return -1;
        }
    }

    while (element_count > 0)
    {
        values[element_count] = element_count;
        element_count--;
    }

    pthread_t tid;
    int ret=pthread_create(&tid, NULL, routine, &values);
    if (ret != 0)
    {
        perror("pthread_create");
        return -1;
    }

    start=true;
    pthread_cond_broadcast(&cv);

    pthread_rwlock_rdlock(&rwlock);
    for (std::map<int, int>::const_iterator it = values.begin(); it != values.end(); it++)
    {
        std::cout << it->first << " " << it->second << std::endl;
    }
    pthread_rwlock_unlock(&rwlock);

    ret=pthread_join(tid, NULL);
    if (ret != 0)
    {
        perror("pthread_join");
        return -1;
    }

    return 0;
}
