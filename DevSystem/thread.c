#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* routine(void* args)
{
    printf("Thread top of stack near %p\n", &args);

    sleep(5);

    //char buffer[0x35000];

    //memset(buffer, 0x65, 0x35000);

}

int main(int argc, char const *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;

    int ret=pthread_attr_init(&attr);

    printf("pthread_attr_init %d \n", ret);

    // 0x80 000 => 524 288 bytes => 0.5 MB
    ret = pthread_attr_setstacksize(&attr, 0x35000);

    printf("pthread_attr_setstacksize %d \n", ret);

    ret=pthread_create(&tid, &attr, routine, NULL);

    printf("pthread_create %d \n", ret);

    pthread_join(tid, NULL);

    ret = pthread_attr_destroy(&attr);

    printf("pthread_attr_destroy %d \n", ret);

    return 0;
}
