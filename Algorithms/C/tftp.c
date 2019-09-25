#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>
#include <signal.h>
#include <sys/time.h>

/*
 * RFC : https://tools.ietf.org/html/rfc1350
 * 
 * 
 */

#define TFTP_DEFAULT_BIND_PORT 69
#define TFTP_RECV_TIMEOUT_SEC 5

#define DATA_BLOCK_SIZE 512

#define THREAD_POOL_MAX 5
#define TFTP_DEFAULT_FOLDER "/tftpboot"

// --------------------------------------------------------------------------------------------

typedef enum
{
    RRQ = 1; // Read Request
    WRQ = 2; // Write Request
    DATA = 3;
    ACK = 4;
    ERROR = 5;
} Tftp_packet_type;

typedef union {
    uint16_t opcode;
    struct
    {
        uint16_t opcode;
        uint8_t filename_and_mode[DATA_BLOCK_SIZE + 2 * sizeof(char)]; // Filename\0 Mode\0  => linux_x86-64.img\0 netascii/octet/mail
    } rd_wr_request;

    struct
    {
        uint16_t opcode; /* DATA */
        uint16_t block_number;
        uint8_t data[DATA_BLOCK_SIZE];
    } data;

    struct
    {
        uint16_t opcode; /* ACK */
        uint16_t block_number;
    } ack;

    struct
    {
        uint16_t opcode; /* ERROR */
        uint16_t error_code;
        uint8_t error_string[DATA_BLOCK_SIZE];
    } error;

} tftp_message;

// --------------------------------------------------------------------------------------------

ssize_t tftp_recv_message(int sock, tftp_message* message, struct sockaddr_in* endpoint_sockaddr, socklen_t* slen)
{
    ssize_t recv_size = recvfrom(sock, message, sizeof(tftp_message), 0, (struct sockaddr *)endpoint_sockaddr, slen);

    if (recv_size < 0 && errno != EAGAIN)
    {
        fprintf(stderr, "recvfrom error : %s\n", strerror(errno));
    }

    return recv_size;
}

int tftp_handle_request()
{
    int is_last_msg=0;
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct timeval tv;
    tv.tv_sec = TFTP_RECV_TIMEOUT_SEC;
    tv.tv_usec = 0;


    int ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    if (ret < 0)
    {
        perror("setsockopt()");
        return -1;
    }

    ssize_t sent_size = tftp_send_data(sock, block_number, data, dlen, client_sock, slen);

    ssize_t recv_size = tftp_recv_message(sock, &m, client_sock, &slen);
    if (recv_size < sizeof(m.data))
    {
        is_last_msg = 1;
    }

    if (recv_size != sizeof())

    if (errno != EAGAIN)
    {
        perror("recv()");
        return -1;
    }
}

/*
protocol        = udp
port            = 69
socket_type     = dgram
wait            = yes
user            = nobody
server          = /usr/sbin/in.tftpd
server_args     = /tftpboot
*/

void usage(const char* prog_name)
{
    fprintf(stderr, "usage : %s -p <port> -f <folder>\n", prog_name);
    fprintf(stderr, "example : %s -p %d -f %s\n", prog_name, TFTP_BIND_PORT, TFTP_DEFAULT_FOLDER);
}

// --------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    uint16_t read_port = TFTP_DEFAULT_BIND_PORT;
    char *folder = TFTP_DEFAULT_FOLDER;

    int c;
    while ((c = getopt(argc, argv, "p:f:")) != -1)
    {
        switch (c)
        {
        case 'p':
            read_port = atoi(optarg);
            break;
        case 'f':
            folder = optarg;
            break;
        case '?':
        default:
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stdout, "[DEBUG] starting tftp server on port : %d using %s base folder\n", read_port, folder);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_sockaddr.sin_port = htons(read_port);

    int ret = bind(sock, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr));
    if (ret != 0)
    {
        fprintf(stderr, "Error binding socket to port : %d [%s]\n", read_port, strerror(errno));
    }

    struct timeval tv;
    tv.tv_sec = TFTP_RECV_TIMEOUT_SEC;
    tv.tv_usec = 0;

    ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (ret != 0)
    {
        fprintf(stderr, "[ERROR] unable to set timeout using setsockopt errno = %s\n", strerror(errno));
    }

    thread_pool* th_pool=thread_pool_init(THREAD_POOL_MAX);
    if (th_pool == NULL)
    {
        fprintf(stderr, "thread_pool_init\n");
    }

    int run=1;
    while (run)
    {
        struct sockaddr_in client_sock;
        socklen_t slen = sizeof(client_sock);
        tftp_message message;

        ssize_t len = tftp_recv_message(sock, &message, &client_sock, &slen));

        // if size 0< or 4<

        if (opcode == (uint16_t)RRQ || opcode == (uint16_t)WRQ)
        {
            int ret = thread_pool_execute(th_pool, tftp_handle_request, args);
            if (ret != 0)
            {

            }
        }
    }

    return 0;
}