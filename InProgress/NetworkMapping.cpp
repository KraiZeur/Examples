/*

Mapping de champs d'un buffer réseau vers Système

Packet* packet = new Packet(buffer);

FieldMapper mapper = new FieldMapper();
mapper.mapField("HostName", );


mapper.getValue("HostName", packet);

*/

class Packet {
private:
    char* buffer;
public:


};

class NetworkMapping {
private:
public:
    template <typename T>
    T getValue(Packet* packet, std::string fieldName);

    template <typename T>
    T getValue(Packet *packet, std::string fieldName);
};


// NetwkrMappingBuilder
// create a networkMapping

int main(void)
{
    char* buffer[512] = generateNetworkBuffer(512);


    Packet* packet;
    packet->setFromBuffer(buffer);

    NetworkMapping mapping = new NetworkMapping();
    mapping.setField<char>("HostName", 1);
    mapping.setField<char>("IpAddress", 1);
    mapping.setField<uint32_t>("Array", 15);

    mapping.build();


    mapping.getField<char>(buffer, "HostName");

}