#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void routine(int signal) {

}

int main(int argc, char const *argv[]) {

	struct sigaction action;
	action.sa_handler = &routine;

	sigemptyset(&(action.sa_mask));

	sigaction(SIGALRM, &action, NULL);

	kill(getpid(), SIGALRM);
	kill(getpid(), SIGUSR1);

	return 0;
}

