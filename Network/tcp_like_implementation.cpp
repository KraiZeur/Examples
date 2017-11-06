// Message based
// Acknowldege system
// Stream oriented
// Window

// Providing Flow Control and Congestion Avoidance Features

// stream oriented
// bidirectional
// acknowledge timer


/*

Sliding windows

bytes :
sent and acknowledge
sent but not acknowledge
not sent but ready
not sent and not ready


SO_SNDBUF = 87380 bytes
SO_RCVBUF = 87380 bytes

*/

#include <tuple>

class NetworkHeader {

};

class Packet {

};

class Member {

};

// handle endianness
// convert packet to class and vice & verca
namespace meta
{
  template <>
  inline auto registerHeader<TCPImplementation>() {
    return Member (
                  Member<uint16_t>("sourcePort", 1, 0),
                  Member<uint16_t>("destPort", 1, 0),
                  Member<uint32_t>("sequenceNumber", 1, 0),
                  Member<uint32_t>("acknowledgeNumber", 1, 0),
                  Member<uint8_t>("dataOffset", 0, 4),
                  Member<uint8_t>("reserved", 0, 6),
                  Member<uint8_t>("controlBits", 0, 6),
                    {
                      Member<uint8_t>("URG", 0, 1),
                      Member<uint8_t>("ACK", 0, 1),
                      Member<uint8_t>("PSH", 0, 1),
                      Member<uint8_t>("RST", 0, 1),
                      Member<uint8_t>("SYN", 0, 1),
                      Member<uint8_t>("FIN", 0, 1)
                   }
                  Member<uint16_t>("window", 1, 0),
                  Member<uint16_t>("checksum", 1, 0),
                  Member<uint16_t>("urgentPointer", 1, 0),
                  Member<uint32_t>("options", 1, 0)
                );
  }
}


class Network {
private:
public:
};


class TCPImplementation : public Network {
private:
public:
};

class SlidingWindow {
/*

  Usable Window = SND.UNA  + SND.WND - SND.NXT

  SND.UNA = 32 [send unackowledged data ptr]
  SND.WND = 20 [max size window]
  SND.NXT = 46 [send next data ptr]


*/
};

class UDPImplementation : public Network {
private:
public:
};

class NetworkSimulator {
private:
  Network* _network;
public:
};