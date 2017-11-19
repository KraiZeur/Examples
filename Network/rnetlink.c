#include <stdio.h>
#include <errno.h>


int main(int argc, char* argv[])
{

    int status = sendmsg(fd, &msg, 0);

    if (status < 0)
    {
        perror("Cannot talk to rtnetlink");
        return -1;
    }

    char buf[16384];
    memset(buf,0,sizeof(buf));

    while (1)
    {
        status = recvmsg(fd, &msg, 0);
        if (status < 0)
        {
            if (errno == EINTR || errno == EAGAIN)
                continue;
            fprintf(stderr, "netlink receive error %s (%d)\n",
                    strerror(errno), errno);
            return -1;
        }
        if (status == 0)
        {
            fprintf(stderr, "EOF on netlink\n");
            return -1;
        }
    }

    return 0;
}