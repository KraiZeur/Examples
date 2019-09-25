#include <stdint.h>

typedef enum
{
    RESERVED = 0,
    SYM_ACTIVE = 1,
    SYM_PASSIVE = 2,
    CLIENT = 3,
    SERVER = 4,
    BROADCAST = 5,
    RESERVED_NTP = 6,
    RESERVED_PRIVATE = 7
} sntp_mode_et;

typedef enum
{
    NO_WARNING=0,
    SPARE_1=1,
    SPARE_2=2,
    ALARM_CONDITION=3
} sntp_leap_indicator_et;

#define SNTP_VERSION_NUMBER 0x04

typedef struct __attribute__((packed))
{
    uint32_t second;
    uint32_t fraction;
} ntp_time_st;

typedef struct __attribute__((packed)) {
    sntp_leap_indicator_et LI : 2;
    unsigned int VN : 2;
    ntp_mode_et Mode : 3;
    uint8_t Stratum;
    uint8_t Poll;
    int8_t Precision;

    int32_t Root_Delay;
    uint32_t Root_Dispersion;
    char Root_Identifier[4]; // bit_string 4bit ASCII string
    ntp_time_st Reference_Timestamp;
    ntp_time_st Originate_Timestamp; // T0
    ntp_time_st Receive_Timestamp;   // T1
    ntp_time_st Transit_Timestamp;   // T2
} sntp_message_st;

void convert_ntp_time_into_unix_time(ntp_time_st *ntp, struct timeval *unix)
{
    unix->tv_sec = ntp->second - 0x83AA7E80; // the seconds from Jan 1, 1900 to Jan 1, 1970
    unix->tv_usec = (uint32_t)((double)ntp->fraction * 1.0e6 / (double)(1LL << 32));
}

void convert_unix_time_into_ntp_time(struct timeval *unix, ntp_time_st *ntp)
{
    ntp->second = unix->tv_sec + 0x83AA7E80;
    ntp->fraction = (uint32_t)((double)(unix->tv_usec + 1) * (double)(1LL << 32) * 1.0e-6);
}

ntp_time_st compute_roundtrip_delay(ntp_time_st T1, ntp_time_st T2, ntp_time_st T3, ntp_time_st T4)
{
    return (T4 - T1) - (T3 - T2);
}

ntp_time_st system_clock_offset(ntp_time_st T1, ntp_time_st T2, ntp_time_st T3, ntp_time_st T4)
{
    return ((T2 - T1) + (T3 - T4)) / 2;
}

void create_client_req(sntp_message_st* msg)
{
    memset(msg, 0x00, sizeof(sntp_message_st));

    msg.LI = NO_WARNING;
    msg.VN = SNTP_VERSION_NUMBER;
    msg.Mode = CLIENT;

    //msg.Transit_Timestamp = XXX; // Should be set at very last
}

sntp_message_st create_server_rep(sntp_message_st *in_msg, ntp_time_st receive_timestamp)
{
    sntp_message_st msg;
    msg.LI = NO_WARNING;
    msg.VN = SNTP_VERSION_NUMBER;
    msg.Mode = CLIENT;

    msg.Originate_Timestamp = in_msg->Transit_Timestamp;
    msg.Receive_Timestamp = receive_timestamp;

    //msg.Transit_Timestamp = XXX; // Should be set at very last
}

void* routine_client(void* args)
{
    while (1)
    {

    }
    return NULL;
}

void* routine_server(void *args)
{
    while (1)
    {

    }
    return NULL;
}

int main(void)
{

    return 0;
}
