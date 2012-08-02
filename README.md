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
