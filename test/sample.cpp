#include "Bitstream.h"

//just to make sure everything compiles without dependencies missing
void testCompile()
{
    uint8_t data[1]; 
    data[0] = 1 << 7;
    BitstreamReader bs(data, 1);
    uint8_t r = bs.get<1>();
    uint8_t data2[1];
    BitstreamWriter bsw(data2, 1);
    bsw.put<1>(1);
}

