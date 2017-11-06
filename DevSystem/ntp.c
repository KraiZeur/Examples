
#include <stdio.h>
#include <unistd.h>

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


/**
 *     T1'       T2'
 *     xxx-------xxx        -- Server
 *    /             \
 *   /               \
 *  /                 \
 * xx-----------------xx    -- Client
 * T1                 T2
 * 
 * 
 */
int main(int argc, char const *argv[]) {


	// NTP CLient/Server Model :

	struct timespec t1, t1_prime;
	struct timespec t2, t2_prime;

	t1 = sendToServer();
	usleep(100); // 100us network latency

	t1_prime = receiveFromServer();

	t2_prime = sendFromServer();
	usleep(100); // 100us network latency

	t2 = receiveFromClient();

	// Delay = (t2-t1 - (t2'-t1')) round trip = le temps passé sur le réseau
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

	//theta = (t1'- t1)/2 + (t1+t2)/2 --> écart theta entre les horloges


	return 0;
}
