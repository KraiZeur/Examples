#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* routine(void* args)
{
  sleep(5);

  return NULL;
}

int main(int argc, char const *argv[])
{

  pthread_t tid;
  pthread_create(&tid, NULL, &routine, NULL);

  sleep(5);

  pthread_join(tid, NULL);

  return 0;
}
