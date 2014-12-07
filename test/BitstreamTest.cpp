#include "Bitstream.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("BitstreamTest: read bits from stream", "[getters]")
{
    SECTION("bits in 1 byte (8 bit width)")
    {
        uint8_t data[1];
        BitstreamReader* bs = new BitstreamReader(data, 1);
        // +----------+
        // |1|000 0000|
        // +----------+
        SECTION("getOneBit")
        {
            data[0] = 1 << 7;
            REQUIRE(0b1 == bs->get<1>());
        }

        // +----------+
        // |11|00 0000|
        // +----------+
        SECTION("getMultipleBit")
        {
            data[0] = 1 << 7 | 1 << 6;
            REQUIRE(0b11 == bs->get<2>());
        }

        // +-----------+
        // |10|01 01|11|
        // +-----------+
        SECTION("getMultipleBitSegments")
        {
            data[0] = 0b10010111;
            REQUIRE(0b10 == bs->get<2>());
            REQUIRE(0b0101 == bs->get<4>());
            REQUIRE(0b11 == bs->get<2>());
        }
        delete bs;
    }

    SECTION("bits in 2 byte (16 bit width)")
    {
        uint8_t data[2];
        BitstreamReader bs(data, 2);
        // +---------+----------+
        // |0000 0011 10|00 0000|
        // +---------+----------+
        SECTION("getBitsIn2Bytes")
        {
            data[0] = 0b00000011;
            data[1] = 0b10000000;
            REQUIRE(0b1110 == bs.get<10>());
        }

        // +---------+----------+
        // |0000 0011 10|10 1110|
        // +---------+----------+
        SECTION("getBitsIn2BytesMultipleTimes")
        {
            data[0] = 0b00000011;
            data[1] = 0b10101110;
            REQUIRE(0b1110 == bs.get<10>());
            REQUIRE(0b101110 == bs.get<6>());
        }
    }

    // +---------+----------+---------+----------+
    // |0000 0000 0000 00|10 0000 0011 1000 00|00|
    // +---------+----------+---------+----------+
    SECTION("multiple bits")
    {
        uint8_t data[4];
        BitstreamReader bs(data, 4);
        data[0] = 0b00000000;
        data[1] = 0b00000010;
        data[2] = 0b00000011;
        data[3] = 0b10000000;

        SECTION("getBitsInMultipleBytes")
        {
            bs.get<14>(); // throw away
            REQUIRE(0b1000000011100000 == bs.get<16>());
        }
        SECTION("getFromTheMiddle")
        {
            REQUIRE(0b1000000011100000 == bs.getWithOffset<16>(14));
            REQUIRE(0b1000000011100000 == bs.getWithOffset<16>(14));//second get should still get the same result
            bs.get<14>(); // throw away
            REQUIRE(0b1000000011100000 == bs.get<16>());//make sure mutable API still works
        }
    }

    //       0          1           2           3           4       5           6       7
    // +-----------+------------+-----------+-----------+---------+---------+---------+---------+
    // |000|1 11|00 0|111|00 0|1 11|00 0|111|000|1 11|00 0001 1100 0111 0001 1100 0111 0001|1100|
    // +-----------+------------+-----------+-----------+---------+---------+---------+---------+
    // | a | b  | c  | d | e  | f  | g  | h | i | j  |         k                           | l  |
    SECTION("getComplexExample")
    {
        uint8_t data[8];
        BitstreamReader bs(data, 8);
        data[0] = 0b00011100;
        data[1] = 0b01110001;
        data[2] = 0b11000111;
        data[3] = 0b00011100;
        data[4] = 0b00011100;
        data[5] = 0b01110001;
        data[6] = 0b11000111;
        data[7] = 0b00011100;
        REQUIRE(0b000 == bs.get<3>());
        REQUIRE(0b111 == bs.get<3>());
        REQUIRE(0b000 == bs.get<3>());
        REQUIRE(0b111 == bs.get<3>());
        REQUIRE(0b000 == bs.get<3>());
        REQUIRE(0b111 == bs.get<3>());
        REQUIRE(0b000 == bs.get<3>());
        REQUIRE(0b111 == bs.get<3>());
        REQUIRE(0b000 == bs.get<3>());
        REQUIRE(0b111 == bs.get<3>());
        REQUIRE(0b000001110001110001110001110001 == bs.get<30>());
        REQUIRE(0b1100 == bs.get<4>());
    }
}

TEST_CASE("BitstreamTest: write bits to stream", "[setters]")
{
    SECTION("up to one byte width")
    {
        uint8_t data[1];
        BitstreamWriter bs(data, 1);
        // +----------+
        // |1|--- ----|
        // +----------+
        SECTION("putOneBit")
        {
            bs.put<1>(1);
            REQUIRE(0b10000000 == data[0]);
        }

        // +-----------+
        // |0|1|00 0000|
        // +-----------+
        SECTION("putOneBitButUseToBigData")
        {
            bs.put<1>(0);
            bs.put<1>(0xFF);
            REQUIRE(0b01000000 == data[0]);
        }

        // +----------+
        // |11|10 1000|
        // +----------+
        SECTION("putTwiceInFirstByte")
        {
            bs.put<2>(0b11);
            bs.put<3>(0b101);
            REQUIRE(0b11101000 == data[0]);
        }
    }
    // +---------+---------+
    // |1111 0000 1111|0000|
    // +---------+---------+
    SECTION("putMoreThan8bits")
    {
        uint8_t data[2];
        BitstreamWriter bs(data, 2);
        bs.put<12>(0xF0F);
        REQUIRE(0b11110000 == data[0]);
        REQUIRE(0b11110000 == data[1]);
    }
    // +---------+---------+---------+----------+
    // |0000 0001 0001 0010 0000 0011 1000 00|--|
    // +---------+---------+---------+----------+
    SECTION("putMoreThanIn2bytes")
    {
        uint8_t data[4];
        BitstreamWriter bs(data, 4);
        bs.put<30>(0b000000010001001000000011100000);
        REQUIRE( 0b00000001 == data[0]);
        REQUIRE( 0b00010010 == data[1]);
        REQUIRE( 0b00000011 == data[2]);
        REQUIRE( 0b10000000 == data[3]);
    }
    // +----------+---------+---------+----------+---------+
    // |01|10 1101 0101|0010 0100 0011 1|000 1101|1000 0110|
    // +----------+---------+---------+----------+---------+
    SECTION("putComplexExample")
    {
        // 11|100111011|0010010000111|001101|1000110
        uint8_t data[5];
        BitstreamWriter bs(data, 5);
        bs.put<2>(0b11);            // 11
        bs.put<9>(0b100111011);     // 11|10 0111 011
        bs.put<13>(0b0010010000111);// 11|10 0111 011|0 01001 0000 111
        bs.put<6>(0b001101);        // 11|10 0111 011|0 01001 0000 111|0 0110 1
        bs.put<7>(0b1000110);       // 11|10 0111 011|0 01001 0000 111|0 0110 1|100 0110
        REQUIRE( 0b11100111 == data[0]);
        REQUIRE( 0b01100100 == data[1]);
        REQUIRE( 0b10000111 == data[2]);
        REQUIRE( 0b00110110 == data[3]);
        REQUIRE( 0b00110000 == data[4]);
    }
    // +---------+---------+---------+---------+---------+---------+---------+---------+---------+
    // |0110|1101 0101 0010 0100 0011 1000 1101 1000 0110 0101 0010 0100 0011 1000 1101 1000|0110|
    // +---------+---------+---------+---------+---------+---------+---------+---------+---------+
    SECTION("put64Bit")
    {
        uint8_t data[9];
        BitstreamWriter bs(data, 9);
        bs.put<4>(0b0110);
        bs.put<64>(0b1101010100100100001110001101100001100101001001000011100011011000);
        bs.put<4>(0b0110);

        REQUIRE( 0b01101101 == data[0]);
        REQUIRE( 0b01010010 == data[1]);
        REQUIRE( 0b01000011 == data[2]);
        REQUIRE( 0b10001101 == data[3]);
        REQUIRE( 0b10000110 == data[4]);
        REQUIRE( 0b01010010 == data[5]);
        REQUIRE( 0b01000011 == data[6]);
        REQUIRE( 0b10001101 == data[7]);
        REQUIRE( 0b10000110 == data[8]);
    }
    SECTION("putAndGetFun")
    {
        uint8_t data[20];
        BitstreamReader bsr(data, 20);
        BitstreamWriter bsw(data, 20);
        uint8_t a = 0b0110;
        uint64_t b = 0b1101010100100100001110001101100001100101001001000011100011011000;
        uint8_t c = 0b10110;
        uint16_t d = 0b1001010110;
        uint32_t e = 0b11011011001010110;

        bsw.put<4>(a);
        bsw.put<64>(b);
        bsw.put<5>(c);
        bsw.put<10>(d);
        bsw.put<17>(e);

        REQUIRE(bsr.get<4>() == a);
        REQUIRE(bsr.get<64>() == b);
        REQUIRE(bsr.get<5>() == c);
        REQUIRE(bsr.get<10>() == d);
        REQUIRE(bsr.get<17>() == e);
    }
}

// TEST_F(BitstreamTest, putMoreThanWriterCanTake) TODO





