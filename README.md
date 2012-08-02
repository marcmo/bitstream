# Bitstream

small utility functions for serialization and de-serialization of data

## Example Usecase

write or read incoming/outgoing datastreams for protocols

## Usage

### Writing bits to a stream

    uint8_t data[1];
    BitstreamWriter bs(data, 1);
    bs.put<2>(0b11);
    bs.put<3>(0b101);

### Reading bits from a stream

    uint8_t data[2]; 
    data[0] = 0b00000011;
    data[1] = 0b10101110;
    ...
    BitstreamReader bs(data, 2);
    uint16_t a = bs.get<10>();
    uint16_t b = bs.get<6>();

### Example: Parsing RTCP Packets

            0                   1                   2                   3
            0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    header|V=2|P|    RC   |   PT=SR=200   |             length            |
          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
          |                         SSRC of sender                        |
          +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
    sender|              NTP timestamp, most significant word             |
    info  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
          |             NTP timestamp, least significant word             |
          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
          |                         RTP timestamp                         |
          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
          |                     sender's packet count                     |
          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
          |                      sender's octet count                     |
          +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
    
Writing the code for the parser is now straight forward:

    uint8_t rtcp_packet[24]; 
    ... // copy data from network receive
    BitstreamReader bs(rtcp_packet, 24);
    uint8_t version = bs.get<2>();
    uint8_t padding = bs.get<1>();
    uint8_t rc = bs.get<5>();
    uint8_t packetType = bs.get<8>();
    uint16_t length = bs.get<16>();
    uint32_t ssrc = bs.get<32>();
    uint64_t ntpTimestamp = bs.get<64>();
    uint32_t rtpTimestamp = bs.get<32>();
    uint32_t packetCount = bs.get<32>();
    uint32_t octetCount = bs.get<32>();

