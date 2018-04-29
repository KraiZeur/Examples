#include <stdio.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <net/if.h>

#include <linux/if_ether.h>

#define IPV4_ETHERTYPE 0x0800
#define ARP_ETHERTYPE  0x0806

struct eth_hdr
{
    unsigned char dmac[6];
    unsigned char smac[6];
    uint16_t ethertype; // < 1500 (length of payload) || > 1500 (type of protocol) 2^16 == 65k
    unsigned char payload[1500];
    uint32_t crc_checksum;
} __attribute__((packed));

struct arp_hdr
{
    uint16_t hwtype;
    uint16_t protype;
    unsigned char hwsize;
    unsigned char prosize;
    uint16_t opcode;
    unsigned char data[];
} __attribute__((packed));

void print_error(const char* str)
{
    fprintf(stderr, str);
}

/*
 * Taken from Kernel Documentation/networking/tuntap.txt
 */
int tun_alloc(char *dev)
{
    struct ifreq ifr;
    int fd, err;

    if ((fd = open("/dev/net/tap", O_RDWR)) < 0)
    {
        print_error("Cannot open TUN/TAP dev");
        exit(1);
    }

    CLEAR(ifr);

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if (*dev)
    {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0)
    {
        print_error("ERR: Could not ioctl tun: %s\n", strerror(errno));
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}

int arp_request(const unsigned char[6] mac_address)
{
    struct eth_hdr hdr;
    hdr.dmac = FF.FF.FF.FF.FF.FF.FF;
    hdr.smac = mac_address;
    hdr.ethertype = ARP_ETHERTYPE;
    hdr.payload = ;
}

int main(void) {


}