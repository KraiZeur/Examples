
#include <stdio.h>
#include "../GeneralC++/time_sub.h"
/**
 * https://fr.wikipedia.org/wiki/Network_Time_Protocol
 */

struct timespec getTime() {
	struct timespec time_h;
	clock_gettime(CLOCK_MONOTONIC, &time_h);

	return time_h;
}

struct timespec sendToServer() {
	return getTime();
}

struct timespec receiveFromServer() {
	return getTime();
}

struct timespec sendFromServer() {
	return getTime();
}

struct timespec receiveFromClient() {
	return getTime();
}

int main(int argc, char const *argv[]) {

	struct timespec t1, t1_prime;
	struct timespec t2, t2_prime;

	t1 = sendToServer();

	t1_prime = receiveFromServer();

	t2_prime = sendFromServer();

	t2 = receiveFromClient();

	// Delay = (t2-t1 - -t2'-t1')
	struct timespec res1, res2;
	tsub(t2, t1, res1);
	tsub(t2_prime, t1_prime, res2);

	struct timespec delay;
	tsub(res1, res2, delay);

	printf("T1    : %ld.%09ld ns\n", t1.tv_sec, t1.tv_nsec);
	printf("T1'   : %ld.%09ld ns\n", t1_prime.tv_sec, t1_prime.tv_nsec);
	printf("T2    : %ld.%09ld ns\n", t2.tv_sec, t2.tv_nsec);
	printf("T2'   : %ld.%09ld ns\n", t2_prime.tv_sec, t2_prime.tv_nsec);
	printf("Delay : %04ld.%09ld ns\n", delay.tv_sec, delay.tv_nsec);

	//theta =


	return 0;
}
