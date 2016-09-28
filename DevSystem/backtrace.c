#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Source example
 * http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes
 * It's important to compile the code with "-g -rdynamic" flags
 *
 * catchsegv ./program
 * LD_PRELOAD=/lib/libSegFault.so program or LD_PRELOAD=/lib64/libSegFault.so
 */

// Define the number of trace displayed by the backtrace
#define SIZE 3

void handler(int signum) {
  void* array[SIZE];
  int size=backtrace(array, SIZE);

  fprintf(stderr, "Size returned : %d\n", size);

  backtrace_symbols_fd(array, SIZE, STDERR_FILENO);

  exit(EXIT_FAILURE);
}

void raise_signal() {
  raise(SIGSEGV);
}

int main(int argc, char const *argv[]) {
  struct sigaction action;
  action.sa_handler = &handler;

  sigaction(SIGSEGV, &action, NULL);

  raise_signal();

  return 0;
}
