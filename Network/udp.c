#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>

#define BUFFER_SIZE 15
#define LISTEN_PORT 5000
#define POLLING_PERIOD_USEC 200000

/**
 *
 * Basic UDP server/client example
 * One thread for server
 * One thread for client
 * Each messages are read from the CLI messages are sent after
 * the carriage return character
 *
 * SOCK_DGRAM: The excess bytes are discarded.
 * In case the receive buffer is lower than received packet
 *
 * bind an address to the current socket (listen)
 *
 * connect connect the socket to a remote computer
 *
 * shutdown close connexion when connect is called
 *
 * SO_SNDBUF this sets the limit on the size of the datagram, i.e. anything larger will be discarded.
 *
 * SO_LINGER SO_KEEPALIVE -> tcp
 *
 * as long as the UDP socket send buffer size is greater than the socket's low-water mark, the UDP socket is
 * always writable
 *
 * if fgets size is more or equal than BUFFER_SIZE the BUFFER_SIZE char is
 * replaced by a 0x00 byte
 *
 * Ex write : AZERTY\n
 * end data = AZERTY\n\0
 *
 *
 * TODO : SO_BROADCAST
 *
 */

 int running=1;

void * server(void * arg) {
	int * fd = (int *)arg;

	struct sockaddr_in listen_address;
	memset(&listen_address, 0x00, sizeof(listen_address));
	listen_address.sin_family = AF_INET;
	listen_address.sin_addr.s_addr = htonl (INADDR_ANY);
	listen_address.sin_port = htons (LISTEN_PORT);
	bind (*fd, (struct sockaddr *)&listen_address, sizeof(listen_address));

	char buffer[BUFFER_SIZE];

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = POLLING_PERIOD_USEC;
	setsockopt(*fd, SOL_SOCKET, SO_RCVTIMEO,(char *)&tv, sizeof(struct timeval));

	while(running) {
		ssize_t size = recv(*fd, buffer, BUFFER_SIZE, MSG_PEEK); // MSG PEEK only reads and store but keep data inside socket

		if (size>0)
			fprintf(stderr, "[SERVER] %ld received from MSG_PEEK\n", size);

		size = recv(*fd, buffer, BUFFER_SIZE, 0);

		if (size>0)
			fprintf(stderr, "[SERVER] %ld received %*s\n", size, (int)(size), buffer);
	}
	pthread_exit(NULL);
}

void * client(void * arg) {
	int * fd = (int *)arg;

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = inet_addr ("127.0.0.1") ;
	serv_addr.sin_port = htons (LISTEN_PORT) ;
	memset (&serv_addr.sin_zero, 0x00, sizeof(serv_addr.sin_zero));
	connect(*fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	int sendbuff=0;
	socklen_t optlen = sizeof(sendbuff);
	int ret=getsockopt(*fd, SOL_SOCKET, SO_SNDBUF, &sendbuff, &optlen);
	fprintf(stderr, "ret = %d getsockopt SO_SNDBUF %d\n", ret, sendbuff);
	ret=getsockopt(*fd, SOL_SOCKET, SO_RCVBUF, &sendbuff, &optlen);
	fprintf(stderr, "ret = %d getsockopt SO_RCVBUF %d\n", ret, sendbuff);

	char buffer[BUFFER_SIZE];

	while (fgets(buffer, BUFFER_SIZE, stdin)) {
		fprintf(stderr, "[CLIENT] %ld sent %*s\n", strlen(buffer), (int)(strlen(buffer)), buffer);
		send(*fd, buffer, strlen(buffer)<BUFFER_SIZE?strlen(buffer):BUFFER_SIZE, 0); // flags
	}
	running=0;
	pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {

	int fd = socket(PF_INET, SOCK_DGRAM, 0);
	int fd2 = socket(PF_INET, SOCK_DGRAM, 0);

	pthread_t server_id, client_id;

	pthread_create(&server_id, NULL, server, &fd);
	pthread_create(&client_id, NULL, client, &fd2);

	pthread_join(server_id, NULL);
	pthread_join(client_id, NULL);

	close(fd);
	shutdown(fd2, SHUT_RDWR);
	close(fd2);

	return 0;
}
