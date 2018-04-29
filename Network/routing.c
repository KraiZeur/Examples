#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <errno.h>
#include <netinet/in.h>
#include <net/route.h>
#if defined(__GLIBC__) && __GLIBC__ >= 2 && __GLIBC_MINOR__ >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h>
#else
#include <sys/types.h>
#include <netinet/if_ether.h>
#endif
/**
* Create socket function
*/
int create_socket()
{

  int sockfd = 0;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1)
  {
    fprintf(stderr, "Could not get socket.\n");
    return -1;
  }

  return sockfd;
}

/**
* Generic ioctrlcall to reduce code size
*/
int generic_ioctrlcall(int sockfd, u_long *flags, struct ifreq *ifr)
{

  if (ioctl(sockfd, (long unsigned int)flags, &ifr) < 0)
  {
    fprintf(stderr, "ioctl: %s\n", (char *)flags);
    return -1;
  }
  return 1;
}

/**
* Set route with metric 100
*/
int set_route(int sockfd, char *gateway_addr, struct sockaddr_in *addr)
{

  struct rtentry route;
  int err = 0;
  memset(&route, 0, sizeof(route));
  addr = (struct sockaddr_in *)&route.rt_gateway;
  addr->sin_family = AF_INET;
  addr->sin_addr.s_addr = inet_addr(gateway_addr);
  addr = (struct sockaddr_in *)&route.rt_dst;
  addr->sin_family = AF_INET;
  addr->sin_addr.s_addr = inet_addr("0.0.0.0");
  addr = (struct sockaddr_in *)&route.rt_genmask;
  addr->sin_family = AF_INET;
  addr->sin_addr.s_addr = inet_addr("0.0.0.0");
  route.rt_flags = RTF_UP | RTF_GATEWAY;
  route.rt_metric = 100;

  if ((err = ioctl(sockfd, SIOCADDRT, &route)) < 0)
  {
    //fprintf(stderr, "ioctl: %s\n", (char *)flags);
    fprintf(stderr, "Error ioctl\n");
    return -1;
  }
  return 1;
}

/**
* Set ip function
*/
int set_ip(char *iface_name, char *ip_addr, char *gateway_addr)
{
  if (!iface_name)
    return -1;

  struct ifreq ifr;
  struct sockaddr_in sin;
  int sockfd = create_socket();

  sin.sin_family = AF_INET;

  // Convert IP from numbers and dots to binary notation
  inet_aton(ip_addr, &sin.sin_addr.s_addr);

  /* get interface name */
  strncpy(ifr.ifr_name, iface_name, IFNAMSIZ);

  /* Read interface flags */
  generic_ioctrlcall(sockfd, (u_long *)"SIOCGIFFLAGS", &ifr);

/*
  * Expected in <net/if.h> according to
  * "UNIX Network Programming".
  */
#ifdef ifr_flags
#define IRFFLAGS ifr_flags
#else /* Present on kFreeBSD */
#define IRFFLAGS ifr_flagshigh
#endif

  // If interface is down, bring it up
  if (ifr.IRFFLAGS | ~(IFF_UP))
  {
    ifr.IRFFLAGS |= IFF_UP;
    generic_ioctrlcall(sockfd, (u_long *)"SIOCSIFFLAGS", &ifr);
  }

  // Set route
  set_route(sockfd, gateway_addr, &sin);

  memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));

  // Set interface address
  if (ioctl(sockfd, SIOCSIFADDR, &ifr) < 0)
  {
    fprintf(stderr, "Cannot set IP address. ");
    perror(ifr.ifr_name);
    return -1;
  }
#undef IRFFLAGS

  return 0;
}

void usage()
{
  const char *usage = {
      "./set_ip [interface] [ip address] [gateway address]\n"};
  fprintf(stderr, "%s", usage);
}

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    usage();
    return -1;
  }
  else
  {

    set_ip(argv[1], argv[2], argv[3]);

    return 0;
  }
}