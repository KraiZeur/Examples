/**
 * https://stackoverflow.com/questions/33323429/tcp-dead-link-detection
 */
 
#define MAX_EVENTS      64
#define CON_RETRY_TIMEOUT   10      /* time to wait in sec until next connect attempt */
#define CON_TIMEOUT     10      /* time in sec after which stop trying to connect */
#define CON_IDLE        10      /* time in sec after which con.  seems to be dead*/
#define CON_INIT_TIMEOUT    1       /* time till connection gets reinitialized after shutdown */
#define FD_INVAL        -1
#define MAXBUF          4096
#define DEFAULT_IP      "127.0.0.1"
#define DEFAULT_PORT        2000
/* #define NDEBUG */
#define _GNU_SOURCE

#define ERROR(exitcode_when_not_zero, message, ...) \
    error_at_line(exitcode_when_not_zero, errno, __FILE__, __LINE__, \
        message, ##__VA_ARGS__)

#define ERROR_GOTO(ziel, exit_status, message, ...) \
    do { \
        ret = exit_status; \
        if (asprintf(&error_msg, message " %s:%s:%u: error: %s", \
            ##__VA_ARGS__, program_invocation_name, \
                __FILE__, __LINE__, strerror(errno)) == -1) \
            ERROR(EXIT_FAILURE, "asprintf ERROR_GOTO"); \
        goto ziel; \
    } while (0)

#define SHUT_SOCKET(fd)                 \
    do {                        \
        if (fd >= 0) {              \
            shutdown(fd, SHUT_RDWR);    \
            close(fd);          \
            fd = -1;            \
        }                   \
    } while (0)

#define FREE(x) \
    do { \
        if (x != NULL) { \
            free(x); \
            x = NULL; \
        } \
    } while (0)

#define con_err(e) (e == ECONNRESET  || e == EPIPE || e == ETIMEDOUT \
            || e == ECONNABORTED || e == ECONNREFUSED \
            || e == ENETUNREACH || e == EHOSTUNREACH \
            || e == ETIMEDOUT || e == EACCES || e == EPERM)

#define epoll_hup(events) (events & EPOLLERR \
            || events & EPOLLHUP || events & EPOLLRDHUP)

#include <locale.h>
#include <signal.h>
#include <pwd.h>
#include <getopt.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <sys/signalfd.h>
#include <time.h>
#include <stdint.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <errno.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <assert.h>

#ifndef strlcpy
#define strlcpy(dst,src,sz) snprintf((dst), (sz), "%s", (src))
#endif

volatile sig_atomic_t sigINT_flag = 0;
volatile sig_atomic_t sigTERM_flag = 0;
const char* program_name    = NULL;
static int tcp_out_sock     = FD_INVAL;
static int tcp_listen_sock  = FD_INVAL;
static int tcp_in_sock      = FD_INVAL;
static int con_timeout_tfd  = FD_INVAL;
static int start_listen_tfd = FD_INVAL;
static unsigned short tcp_in_port = 0;
static unsigned short output_device_port = DEFAULT_PORT;
static char output_device_ip[INET_ADDRSTRLEN] = DEFAULT_IP;
int epollfd;

void sigfuncALL(int);
static inline void help(void);
static inline int is_sock_err(int);
static inline int handle_events(int, struct epoll_event *);
static inline int set_sock_opt(int);
static inline int try_connect(char *ip, unsigned short port);
static inline int msg_sock_open(FILE *stream, int sock, int epoll_fd);
static inline int msg_sock_close(FILE *stream, int sock, int epoll_fd);
static inline int start_listen(unsigned short port);

int main(int argc, char **argv)
{
    int next_option;

    struct sigaction act;
    act.sa_flags = 0;

    if (sigemptyset(&act.sa_mask) != 0)
        perror("sigemptyset");

    const char* short_options = "s:p:o:h";
    const struct option long_options[] = {
        { "server", 1, NULL, 's' },
        { "out-port", 1, NULL, 'p' },
        { "out-ip", 1, NULL, 'o' },
        { "help", 0, NULL, 'h' },
        { NULL, 0, NULL, 0 }
    };

    program_name = basename(argv[0]);

    do {
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);
        switch (next_option) {
        case 'h':
            help();
            exit(EXIT_SUCCESS);
        case 's':
            tcp_in_port = (unsigned short) atoi(optarg);
            break;
        case 'p':
            output_device_port = atoi(optarg);
            break;
        case 'o':
            strlcpy(output_device_ip, optarg, INET_ADDRSTRLEN);
            break;
        case '?':
            printf("%s: invalid option: %c\nTry %s --help for more information.\n",
                program_name, next_option, program_name);
            exit(EXIT_FAILURE);
        case -1:
            break;
        default:
            abort();
        }
    } while (next_option != -1);

    int ret = EXIT_SUCCESS; /* für ERROR_GOTO */
    char *error_msg = NULL; /* für ERROR_GOTO */
    int retval, nfds;
    struct epoll_event ev, events[MAX_EVENTS];

    epollfd = epoll_create1(0);
    if (epollfd == -1)
        ERROR_GOTO(done, EXIT_FAILURE, "");

    act.sa_handler = sigfuncALL;
    if (sigaction(SIGTERM, &act, NULL) != 0)
        ERROR_GOTO(done, EXIT_FAILURE, "sigaction SIGTERM");

    act.sa_handler = sigfuncALL;
    if (sigaction(SIGINT, &act, NULL) != 0)
        ERROR_GOTO(done, EXIT_FAILURE, "sigaction SIGINT");

    act.sa_handler = SIG_IGN;
    if (sigaction(SIGPIPE, &act, NULL) != 0)
        ERROR_GOTO(done, EXIT_FAILURE, "sigaction SIGPIPE");

    if (tcp_in_port != 0) {
        static struct itimerspec its_listen = {
            .it_value.tv_sec = 1,
        };

        start_listen_tfd = timerfd_create(CLOCK_MONOTONIC, 0);
        if (start_listen_tfd == -1)
            ERROR_GOTO(done, EXIT_FAILURE,
                "timerfd_create(start_listen_tfd)");

        if (timerfd_settime(start_listen_tfd, 0, &its_listen, NULL) == -1)
            ERROR_GOTO(done, EXIT_FAILURE, "");

        ev.events = EPOLLIN;
        ev.data.fd = start_listen_tfd;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, start_listen_tfd, &ev) == -1)
            ERROR_GOTO(done, EXIT_FAILURE, "");
    } else {
        static struct itimerspec its_timeout = {
            .it_value.tv_sec = CON_INIT_TIMEOUT,
            .it_interval.tv_sec = CON_TIMEOUT
        };

        con_timeout_tfd = timerfd_create(CLOCK_MONOTONIC, 0);
        if (con_timeout_tfd == -1)
            ERROR_GOTO(done, EXIT_FAILURE,
                "timerfd_create(con_timeout_tfd)");

        if (timerfd_settime(con_timeout_tfd, 0, &its_timeout, NULL) == -1)
            ERROR_GOTO(done, EXIT_FAILURE, "timerfd_settime(con_retry_tfd)");

        ev.events = EPOLLIN;
        ev.data.fd = con_timeout_tfd;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, con_timeout_tfd, &ev) == -1)
            ERROR_GOTO(done, EXIT_FAILURE, "");
    }

    fprintf(stderr, "ip:%s port: %hu\n",
        output_device_ip, output_device_port);

/* mainloop: */
    while (!sigTERM_flag && !sigINT_flag) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1 && errno == EINTR)
            continue;
        else if (nfds == -1)
            ERROR_GOTO(done, EXIT_FAILURE, "");

        for (int n = 0; n < nfds; n++) {
            retval = handle_events(epollfd, &events[n]);
            if (retval == -1 && errno == EINTR)
                break;
            else if (retval == -1)
                ERROR_GOTO(done, EXIT_FAILURE, "");
        }
    } /* end mainloop */

done:
    puts("Closing Application properly...\n");

    close(con_timeout_tfd);
    close(start_listen_tfd);

    if (ret != EXIT_SUCCESS)
        fprintf(stderr, "%s\n", error_msg);

    FREE(error_msg);

    puts("Terminate " PROG_NAME "\n");

    return EXIT_SUCCESS;
} /* End main() */

static inline void help(void)
{
    printf(
        "Usage: %s [OPTION]\n"
        "Does something.\n\n"
        "Options:\n"
        "  -s\t--server   \tstart as server\n"
        "  -p\t--out-port\t port to connect to\n"
        "  -o\t--out-ip  \t ip to connect to\n"
        "  -h\t--help    \tDisplay this help\n"
        , program_name
    );

    exit(EXIT_SUCCESS);
}

void sigfuncALL(int sig) {
    switch (sig) {
        case SIGINT:
            sigINT_flag = 1;
            break;
        case SIGTERM:
            sigTERM_flag = 1;
            break;
        default:
            break;
    }
}

static inline int try_connect(char *ip, unsigned short port)
{
    int ret, retval, socket_fd;
    int yes = true;
    int value;
    struct sockaddr_in tcp_addr;

    socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (socket_fd == -1)
        return -1;

    if (set_sock_opt(socket_fd) == -1)
        return -1;

    bzero(&tcp_addr, sizeof(tcp_addr));
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_addr.s_addr = inet_addr(ip);
    tcp_addr.sin_port = htons(port);

    retval = connect(socket_fd, (struct sockaddr *) &tcp_addr,
                sizeof(tcp_addr));
    if (retval == -1 && errno == EINPROGRESS)
        ;
    else if (retval == -1) {
        close(socket_fd);
        return -1;
    }

    return socket_fd;
}

static inline int
handle_events(int epoll_fd, struct epoll_event *event)
{
    int ret = 0;
    int retval;
    struct epoll_event ev;
    int bytes_send, bytes_received;
    char buffer[MAXBUF] = {};

    if (epoll_hup(event->events)) {
        fputs("BROKEN\n", stderr);
        msg_sock_close(stderr, event->data.fd, epoll_fd);
    } else if (event->data.fd == tcp_out_sock && event->events & EPOLLOUT) {
        /* tcp output connection established */
        ret = is_sock_err(tcp_out_sock);
        if (ret)
            msg_sock_close(stderr, tcp_out_sock, epoll_fd);
        else
            msg_sock_open(stderr, tcp_out_sock, epoll_fd);
    } else if (event->data.fd == tcp_out_sock && event->events & EPOLLIN) {
        retval = recv(tcp_out_sock, buffer, MAXBUF, MSG_DONTWAIT);
        if (retval > 0) {
            /* TODO */
        } else if (retval == -1 && !(errno == EAGAIN || errno == EWOULDBLOCK)) {
            ERROR(0, " ");
            ret = -1;
            goto done;
        } else if (retval <= 0)
            msg_sock_close(stderr, tcp_out_sock, epoll_fd);
    } else if (event->data.fd == tcp_listen_sock && event->events & EPOLLIN) {
        tcp_in_sock = accept4(tcp_listen_sock, NULL, NULL, SOCK_NONBLOCK);
        if (tcp_in_sock == -1) {
            ret = -1;
            goto done;
        }

        retval = set_sock_opt(tcp_in_sock);
        if (retval == -1) {
            ret = -1;
            goto done;
        }

        /* stop listen */
        msg_sock_close(stderr, tcp_listen_sock, epoll_fd);
        msg_sock_open(stderr, tcp_in_sock, epoll_fd);
    } else if (event->data.fd == tcp_in_sock && event->events & EPOLLIN) {
        while (0 < (bytes_received = recv(tcp_in_sock, buffer, MAXBUF, MSG_DONTWAIT))) {
            /* TODO */
        }

        if (bytes_received == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        } else if (bytes_received == 0 || (bytes_received == -1 && con_err(errno))) {
            msg_sock_close(stderr, tcp_in_sock, epoll_fd);
        } else if (bytes_received == -1) {
            ret = -1;
            goto done;
        }
    } else if (event->data.fd == con_timeout_tfd && event->events & EPOLLIN) {
        uint64_t numExp;
        if (read(con_timeout_tfd, &numExp, sizeof(uint64_t)) == -1) {
            ret = -1;
            goto done;
        }

        msg_sock_close(stderr, tcp_out_sock, epoll_fd);

        errno = 0;
        tcp_out_sock = try_connect(output_device_ip, output_device_port);
        if (tcp_out_sock == -1 && (con_err(errno))) {
            msg_sock_close(stderr, tcp_out_sock, epoll_fd);
            goto done;
        } else if (tcp_out_sock == -1) {
            ret = -1;
            goto done;
        }

        ev.events = EPOLLOUT;
        ev.data.fd = tcp_out_sock;
        retval = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_out_sock, &ev);
        if (retval == -1) {
            ret = -1;
            goto done;
        }
    } else if (event->data.fd == start_listen_tfd && (event->events & EPOLLIN)) {
        uint64_t numExp;
        retval = read(start_listen_tfd, &numExp, sizeof(uint64_t));
        if (retval == -1) {
            ret = -1;
            goto done;
        }

        fprintf(stdout, "start to listen on port: %hu\n", tcp_in_port);

        retval = start_listen(tcp_in_port);
        if (retval == -1 && errno != EADDRINUSE) {
            ret = -1;
            goto done;
        } else if (retval == -1)
            msg_sock_close(stderr, tcp_listen_sock, epoll_fd);

        ev.events = EPOLLIN;
        ev.data.fd = tcp_listen_sock;
        retval = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_listen_sock, &ev);
        if (retval == -1) {
            ret = -1;
            goto done;
        }
    }

done:
    return ret;
}

static inline int set_sock_opt(int socket_fd)
{
    int value;
    int yes = true;
    int ret = 0;
    int retval;

    /* IPPROTO_TCP causes endpoint not connected error on 3.13 ubuntu  */
    retval = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR,
                &yes, sizeof(yes));
    if (retval == -1) {
        ret = -1;
        goto done;
    }

    yes = true;
    retval = setsockopt(socket_fd, SOL_SOCKET, SO_KEEPALIVE,
                &yes, sizeof(yes));
    if (retval == -1) {
        ret = -1;
        goto done;
    }

    yes = true;
    retval = setsockopt(socket_fd, IPPROTO_IP, IP_RECVERR,
                &yes, sizeof(yes));
    if (retval == -1) {
        ret = -1;
        goto done;
    }

    /* keepalive probes */
    value = 2;
    retval = setsockopt(socket_fd, IPPROTO_TCP, TCP_KEEPCNT,
                &value, sizeof(value));
    if (retval == -1) {
        ret = -1;
        goto done;
    }

    /* seconds of idle before sending keepalive probes */
    value = 10;
    retval = setsockopt(socket_fd, IPPROTO_TCP, TCP_KEEPIDLE,
                &value, sizeof(value));
    if (retval == -1) {
        ret = -1;
        goto done;
    }

    /* seconds between keepalive probes */
    value = 1;
    retval = setsockopt(socket_fd, IPPROTO_TCP, TCP_KEEPINTVL,
                &value, sizeof(value));
    if (retval == -1) {
        ret = -1;
        goto done;
    }

done:
    return ret;
}

// if you're performing asynchronous operations on sockets, you may need to use SO_ERROR
static inline int is_sock_err(int socket)
{
    socklen_t optlen = sizeof(int);
    int optval;

    if (getsockopt(socket, SOL_SOCKET, SO_ERROR, (void*) &optval, &optlen))
        return -1;

    if (optval)
        return optval;

    return 0;
}

static inline int
msg_sock_close(FILE *stream, int sock, int epoll_fd)
{
    if (!stream /*|| sock < 0*/) {
        errno = EINVAL;
        return -1;
    }

    struct sockaddr_in peer_addr = {};
    socklen_t peer_addr_len = sizeof(struct sockaddr_in);
    int ret = 0;
    struct epoll_event ev;
    socklen_t optlen = sizeof(int);
    int optval;

    if (!(sock < 0) || errno == 0) {
        ret = getpeername(sock, (struct sockaddr *) &peer_addr,
            &peer_addr_len);
        if (ret == -1 && !(errno == ENOTSOCK || errno == ENOTCONN))
            return -1;

        ret = getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*) &optval, &optlen);
        if (ret == -1 && errno != ENOTSOCK)
            return -1;
    }

    if (sock == tcp_out_sock) {
        fprintf(stream, "tcp output to: %s:%hu closed. Reason: %s\n",
            inet_ntoa(peer_addr.sin_addr), peer_addr.sin_port,
                errno == 0 ? strerror(optval) : strerror(errno));

        /* arm */
        struct itimerspec its_timeout = {
            .it_value.tv_sec = CON_INIT_TIMEOUT,
            .it_interval.tv_sec = CON_TIMEOUT
        };

        if (timerfd_settime(con_timeout_tfd, 0, &its_timeout, NULL) == -1)
            return -1;

        SHUT_SOCKET(tcp_out_sock);

        if (tcp_in_sock != FD_INVAL
            && epoll_ctl(epoll_fd, EPOLL_CTL_DEL,
                    tcp_in_sock, &ev) == -1)
                        return -1;
    } else if (sock == tcp_in_sock) {
        fprintf(stream, "tcp input from: %s:%hu closed. Reason: %s\n",
            inet_ntoa(peer_addr.sin_addr), peer_addr.sin_port, strerror(optval));
        SHUT_SOCKET(tcp_in_sock);

        /* arm */
        struct itimerspec its_retry = {
            .it_value.tv_sec = CON_INIT_TIMEOUT};

        if (timerfd_settime(start_listen_tfd, 0, &its_retry, NULL) == -1)
            return -1;
    } else if (sock == tcp_listen_sock) {
        SHUT_SOCKET(tcp_listen_sock);
        fprintf(stream, "tcp listen closed\n");

        if (tcp_in_sock == FD_INVAL) {
            /* arm */
            struct itimerspec its_retry = {
                .it_value.tv_sec = CON_INIT_TIMEOUT};

            if (timerfd_settime(start_listen_tfd, 0, &its_retry, NULL) == -1)
                return -1;
        }
    } else {
        close(sock);
        sock = FD_INVAL;
    }

    return 0;
}

static inline int
msg_sock_open(FILE *stream, int sock, int epoll_fd)
{
    if (!stream || sock < 0) {
        errno = EINVAL;
        return -1;
    }

    struct sockaddr_in peer_addr = {};
    socklen_t peer_addr_len = sizeof(struct sockaddr_in);
    int ret = 0;
    struct epoll_event ev;
    int retval;

    ret = getpeername(sock, (struct sockaddr *) &peer_addr,
                &peer_addr_len);
    if (ret == -1 && errno == ENOTSOCK)
        return -1;

    if (sock == tcp_out_sock) {
        fprintf(stream, "tcp output to: %s:%hu opened\n",
            inet_ntoa(peer_addr.sin_addr), peer_addr.sin_port);

        /* disarm */
        struct itimerspec its_timeout = {
            .it_value.tv_sec = 0,
            .it_value.tv_nsec = 0
        };

        if (timerfd_settime(con_timeout_tfd, 0, &its_timeout, NULL) == -1)
            return -1;

        ev.events = EPOLLIN;
        ev.data.fd = tcp_out_sock;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, tcp_out_sock, &ev) == -1)
            return -1;

        if (tcp_in_sock != FD_INVAL) {
            ev.events = EPOLLIN;
            ev.data.fd = tcp_in_sock;
            retval = epoll_ctl(epoll_fd, EPOLL_CTL_ADD,
                        tcp_in_sock, &ev);
            if (retval == -1)
                return -1;
        }
    } else if (sock == tcp_in_sock) {
        fprintf(stream, "tcp input from: %s:%hu opened\n",
            inet_ntoa(peer_addr.sin_addr), peer_addr.sin_port);

        ev.events = EPOLLIN;
        ev.data.fd = tcp_in_sock;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_in_sock, &ev) == -1)
            return -1;
    } else if (sock == tcp_listen_sock) {
        ev.events = EPOLLIN;
        ev.data.fd = tcp_listen_sock;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_listen_sock, &ev) == -1)
            return -1;

        /* disarm */
        struct itimerspec its_listen = {
            .it_value.tv_sec = 0,
            .it_interval.tv_sec = 0
        };

        if (timerfd_settime(start_listen_tfd, 0, &its_listen, NULL) == -1) {
            return -1;
        }
    }

    return 0;
}

static inline int
start_listen(unsigned short port)
{
    assert(tcp_listen_sock == FD_INVAL);

    int flag = 1;
    struct sockaddr_in sock_addr;
    int retval, ret = 0;

    tcp_listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_listen_sock == -1)
        return -1;

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    sock_addr.sin_port = htons(port);

    retval = set_sock_opt(tcp_listen_sock);
    if (retval == -1) {
        ret = -1;
        goto done;
    }

    retval = bind(tcp_listen_sock, (struct sockaddr *) &sock_addr,
            sizeof(sock_addr));
    if (retval == -1) {
        ret = -1;
        goto done;
    }

    retval = listen(tcp_listen_sock, 5);
    if (retval == -1) {
        ret = -1;
        goto done;
    }

done:
    if (ret == -1)
        SHUT_SOCKET(tcp_listen_sock);

    return ret;
}
