#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <arpa/inet.h> //htons...

typedef struct __attribute__((packed))
{
    struct __attribute__((packed))
    {
        uint16_t x;
        uint32_t y;
    } header;
    char data[];
} payload_t;

typedef enum data_type
{
    UNDEF = 0,
    INT16 = 1,
    INT32 = 2,
    INT64 = 3,
} data_type_e;

typedef struct
{
    data_type_e data_type;
    uint32_t offset;
} encoding_t;

typedef struct
{
    size_t payload_size;
    const encoding_t *encoding;
    payload_t *payload;
} packet_t;

static const encoding_t packet_encoding[] = {
    {INT16, offsetof(payload_t, header.x)},
    {INT32, offsetof(payload_t, header.y)},
    {UNDEF}};

typedef struct udp_socket_r{
    struct {
        const char* addr;
        uint16_t port;
        int socket;
        char* buffer;
        size_t buffer_size;
    } private;
    int (*create)(struct udp_socket_r*, const char*, uint16_t);
    int (*read)(struct udp_socket_r*);
    int (*read_callback)(packet_t*);
    void(*convert_network_to_host)(payload_t*); // would be better to have granularity here (avoid referencing payload_t)
    void(*convert_host_to_network)(payload_t*); // would be better to have granularity here
    int (*close)(struct udp_socket_r*);
} udp_socket_r_t;


int _create_udp_socket(udp_socket_r_t* this, const char* address, uint16_t port)
{
    this->private.socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->private.socket == -1)
    {
        perror("socket()");
        return -1;
    }

    if (this->private.buffer == NULL)
    {
        this->private.buffer = malloc(this->private.buffer_size);
    }

    //bind();
    return 0;
}

int _read_udp_socket(udp_socket_r_t* this)
{
    size_t size_read = recv(this->private.socket, this->private.buffer, this->private.buffer_size, 0);
    if (size_read == -1)
    {
        perror("read()");
        return -1;
    }
    else
    {
        packet_t packet;
        packet.payload = (payload_t *)this->private.buffer;
        packet.payload_size = size_read;
        packet.encoding = packet_encoding;

        this->convert_network_to_host(packet.payload);
        return this->read_callback(&packet);
    }
    return 0;
}

int _close_udp_socket(udp_socket_r_t *this)
{
    if (this->private.socket!=-1)
    {
        int ret = close(this->private.socket);
        if (ret!=0)
        {
            perror("close()");
            return ret;
        }
    }
    return 0;
}

// ------------------

// Could also iterate through enum
// mapping between enum and enconding
// if (enum.data_type == X)
//   call callback to convert data
void _convert_network_to_host(payload_t* payload)
{
    const encoding_t* e=NULL;
    for (e = packet_encoding; e->data_type != UNDEF; ++e)
    {
        switch(e->data_type)
        {
            case INT16:
            {
                uint16_t* data = (uint16_t*)( ((void*)payload) + e->offset);
                uint16_t converted_data = ntohs(*data);

                *data = converted_data;
            }
            break;
            case INT32:
            {
                uint32_t *data = (uint32_t *)(((void *)payload) + e->offset);
                uint32_t converted_data = ntohl(*data);

                *data = converted_data;
            }
            break;
        }
    }
}

void _convert_host_to_network(payload_t *payload)
{
    const encoding_t *e = NULL;
    for (e = packet_encoding; e->data_type != UNDEF; ++e)
    {
        switch (e->data_type)
        {
        case INT16:
        {
            uint16_t *data = (uint16_t *)(((void *)payload) + e->offset);
            uint16_t converted_data = htons(*data);

            *data = converted_data;
        }
        break;
        case INT32:
        {
            uint32_t *data = (uint32_t *)(((void *)payload) + e->offset);
            uint32_t converted_data = htonl(*data);

            *data = converted_data;
        }
        break;
        }
    }
}

// ------------- USER

int on_read_callback(packet_t* packet)
{
    printf("0x%X - 0x%X\n", packet->payload->header.x, packet->payload->header.y);

    return 0;
}

// ---------------------------------------

int main(int argc, char* argv[])
{
    // Alloc buffer on socket creation
    udp_socket_r_t sock_r = {
        .private = {
            .buffer = NULL,
            .buffer_size = 1024,
        },
        .create = _create_udp_socket,
        .read = _read_udp_socket,
        .convert_network_to_host = _convert_network_to_host,
        .convert_host_to_network = _convert_host_to_network,
        .read_callback = on_read_callback,
        .close = _close_udp_socket,
    };

    sock_r.create(&sock_r, "127.0.0.1", 9055);

    uint16_t data=0xFECA;
    uint32_t data2=0xA1ACFF1E;

    memcpy(sock_r.private.buffer, &data, sizeof(uint16_t));
    memcpy((sock_r.private.buffer+sizeof(uint16_t)), &data2, sizeof(uint32_t));

    int run=1;
    while (run)
    {
        sock_r.read(&sock_r);
    }

    sock_r.close(&sock_r);

    return 0;
}