#ifndef BITSTREAM_H_
#define BITSTREAM_H_

#include <stdint.h>
#include <stdio.h>
#include <algorithm>
#include <cassert>

template<size_t WIDTH>
struct ones;
template<>
struct ones<1>
{
    enum { value = 1 };
};
template<size_t WIDTH>
struct ones
{
    enum { value = 1 << (WIDTH-1) | ones<WIDTH-1>::value};
};

template<int N>
struct WidthType;
template<>
struct WidthType<8>
{
    typedef uint8_t Type;
};
template<>
struct WidthType<16>
{
    typedef uint16_t Type;
};
template<>
struct WidthType<24>
{
    typedef uint32_t Type;
};
template<>
struct WidthType<32>
{
    typedef uint32_t Type;
};

template<size_t N>
struct CalculateWidth
{
    enum { value = 8 * ((N-1) / 8 + 1)};
};

template<size_t N>
struct GetWidthType
{
    enum { width = CalculateWidth<N>::value };
    typedef typename WidthType<width>::Type Type;
};
class Bitstream
{
public:
    Bitstream (const uint8_t* data, uint32_t length)
        :   mData(data),
            mLength(length),
            mOffset(0)
    {}
    virtual ~Bitstream ()
    {}

    // get a byte where all bits are set
    // starting with the leftmost bit (from)
    // till the the from+length bit
    // e.g. from=0,to=7 => all bits => 0b11111111
    uint8_t mask(uint8_t from, uint8_t length)
    {
        assert((from + length) <= 8);
        uint8_t res = 0;
        for (int i = 0; i < length; ++i)
        {
            res |= 1 << i;
        }
        res = res << 8 - (from + length);
        return res;
    }

    template<int N>
    typename GetWidthType<N>::Type get()
    {
        typename GetWidthType<N>::Type res = 0;
        
        int index = (mOffset)/8; // [0,]
        int offsetInByte = (mOffset)%8; // [0,7]
        int remainingInFirstByte = 8 - offsetInByte; // [1,6]
        int bitsInFirstByte = std::min(N,remainingInFirstByte);
        res |= mData[index++] & mask(offsetInByte, bitsInFirstByte);
        mOffset += bitsInFirstByte;
        int remainingBits = N - bitsInFirstByte;
        if (!remainingBits)
        {
            res >>= 8 - (N + offsetInByte);
        }
        while (remainingBits > 0)
        {
            if(remainingBits <= 8)
            {
                // process last byte
                res <<= remainingBits; //make room for outstanding bits
                uint8_t masked = mData[index++] & (mask(0,remainingBits));
                res |= masked >> (8 - remainingBits);
                mOffset += remainingBits;
                remainingBits = 0;
            }
            else
            {
                res <<= std::min(8,remainingBits); //make room for outstanding bits
                res |= mData[index++];
                mOffset += 8;
                remainingBits -= 8;
            }
        }
        return res;
    }

private:
    const uint8_t* mData;
    uint32_t mLength;
    uint32_t mOffset;
};

#endif // BITSTREAM_H_
