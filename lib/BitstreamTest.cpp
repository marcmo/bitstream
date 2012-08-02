#include "gtest/gtest.h"
#include "Bitstream.h"

class BitstreamTest : public ::testing::Test
{
public:

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

};

// +----------+
// |1|000 0000|
// +----------+
TEST_F(BitstreamTest, getOneBit)
{
    uint8_t data[1]; 
    data[0] = 1 << 7;
    BitstreamReader bs(data, 1);
    uint8_t r = bs.get<1>();
    EXPECT_EQ(0b1, r);
}

// +----------+
// |11|00 0000|
// +----------+
TEST_F(BitstreamTest, getMultipleBit)
{
    uint8_t data[1]; 
    data[0] = 1 << 7 | 1 << 6;
    BitstreamReader bs(data, 1);
    uint8_t r = bs.get<2>();
    EXPECT_EQ(0b11, r);
}

// +-----------+
// |10|01 01|11|
// +-----------+
TEST_F(BitstreamTest, getMultipleBitSegments)
{
    uint8_t data[1]; 
    data[0] = 0b10010111;
    BitstreamReader bs(data, 1);
    uint8_t a = bs.get<2>();
    EXPECT_EQ(0b10, a);
    uint8_t b = bs.get<4>();
    EXPECT_EQ(0b0101, b);
    uint8_t c = bs.get<2>();
    EXPECT_EQ(0b11, c);
}

// +---------+----------+
// |0000 0011 10|00 0000|
// +---------+----------+
TEST_F(BitstreamTest, getBitsIn2Bytes)
{
    uint8_t data[2]; 
    data[0] = 0b00000011;
    data[1] = 0b10000000;
    BitstreamReader bs(data, 2);
    uint16_t a = bs.get<10>();
    EXPECT_EQ(0b1110, a);
}

// +---------+----------+
// |0000 0011 10|10 1110|
// +---------+----------+
TEST_F(BitstreamTest, getBitsIn2BytesMultipleTimes)
{
    uint8_t data[2]; 
    data[0] = 0b00000011;
    data[1] = 0b10101110;
    BitstreamReader bs(data, 2);
    uint16_t a = bs.get<10>();
    EXPECT_EQ(0b1110, a);
    uint16_t b = bs.get<6>();
    EXPECT_EQ(0b101110, b);
}

// +---------+----------+---------+----------+
// |0000 0000 0000 00|10 0000 0011 1000 00|00|
// +---------+----------+---------+----------+
TEST_F(BitstreamTest, getBitsInMultipleBytes)
{
    uint8_t data[4]; 
    data[0] = 0b00000000;
    data[1] = 0b00000010;
    data[2] = 0b00000011;
    data[3] = 0b10000000;
    BitstreamReader bs(data, 4);
    bs.get<14>(); // throw away
    uint16_t a = bs.get<16>();
    EXPECT_EQ(0b1000000011100000, a);
}

//       0          1           2           3           4       5           6       7
// +-----------+------------+-----------+-----------+---------+---------+---------+---------+ 
// |000|1 11|00 0|111|00 0|1 11|00 0|111|000|1 11|00 0001 1100 0111 0001 1100 0111 0001|1100| 
// +-----------+------------+-----------+-----------+---------+---------+---------+---------+ 
// | a | b  | c  | d | e  | f  | g  | h | i | j  |         k                           | l  |
TEST_F(BitstreamTest, getComplexExample)
{
    uint8_t data[8]; 
    data[0] = 0b00011100;
    data[1] = 0b01110001;
    data[2] = 0b11000111;
    data[3] = 0b00011100;
    data[4] = 0b00011100;
    data[5] = 0b01110001;
    data[6] = 0b11000111;
    data[7] = 0b00011100; 
    BitstreamReader bs(data, 8);
    EXPECT_EQ(0b000, bs.get<3>());
    EXPECT_EQ(0b111, bs.get<3>());
    EXPECT_EQ(0b000, bs.get<3>());
    EXPECT_EQ(0b111, bs.get<3>());
    EXPECT_EQ(0b000, bs.get<3>());
    EXPECT_EQ(0b111, bs.get<3>());
    EXPECT_EQ(0b000, bs.get<3>());
    EXPECT_EQ(0b111, bs.get<3>());
    EXPECT_EQ(0b000, bs.get<3>());
    EXPECT_EQ(0b111, bs.get<3>());
    EXPECT_EQ(0b000001110001110001110001110001, bs.get<30>());
    EXPECT_EQ(0b1100, bs.get<4>());
}

// +----------+
// |1|--- ----|
// +----------+
TEST_F(BitstreamTest, putOneBit)
{
    uint8_t data[1];
    BitstreamWriter bs(data, 1);
    bs.put<1>(1);
    EXPECT_EQ(0b10000000, data[0]);
}

// +-----------+
// |0|1|00 0000|
// +-----------+
TEST_F(BitstreamTest, putOneBitButUseToBigData)
{
    uint8_t data[1];
    BitstreamWriter bs(data, 1);
    bs.put<1>(0);
    bs.put<1>(0xFF);
    EXPECT_EQ(0b01000000, data[0]);
}

// +----------+
// |11|10 1000|
// +----------+
TEST_F(BitstreamTest, putTwiceInFirstByte)
{
    uint8_t data[1];
    BitstreamWriter bs(data, 1);
    bs.put<2>(0b11);
    bs.put<3>(0b101);
    EXPECT_EQ(0b11101000, data[0]);
}
// +---------+---------+
// |1111 0000 1111|0000|
// +---------+---------+
TEST_F(BitstreamTest, putMoreThan8bits)
{
    uint8_t data[2];
    BitstreamWriter bs(data, 2);
    bs.put<12>(0xF0F);
    EXPECT_EQ(0b11110000, data[0]);
    EXPECT_EQ(0b11110000, data[1]);
}
// +---------+---------+---------+----------+
// |0000 0001 0001 0010 0000 0011 1000 00|--|
// +---------+---------+---------+----------+
TEST_F(BitstreamTest, putMoreThanIn2bytes)
{
    uint8_t data[4]; 
    BitstreamWriter bs(data, 4);
    bs.put<30>(0b000000010001001000000011100000);
    EXPECT_EQ( 0b00000001, data[0]);
    EXPECT_EQ( 0b00010010, data[1]);
    EXPECT_EQ( 0b00000011, data[2]);
    EXPECT_EQ( 0b10000000, data[3]);
}
// +----------+---------+---------+----------+---------+
// |01|10 1101 0101|0010 0100 0011 1|000 1101|1000 0110|
// +----------+---------+---------+----------+---------+
TEST_F(BitstreamTest, putComplexExample)
{
// 11|100111011|0010010000111|001101|1000110
    uint8_t data[5]; 
    BitstreamWriter bs(data, 5);
    bs.put<2>(0b11);            // 11
    bs.put<9>(0b100111011);     // 11|10 0111 011
    bs.put<13>(0b0010010000111);// 11|10 0111 011|0 01001 0000 111
    bs.put<6>(0b001101);        // 11|10 0111 011|0 01001 0000 111|0 0110 1
    bs.put<7>(0b1000110);       // 11|10 0111 011|0 01001 0000 111|0 0110 1|100 0110
    EXPECT_EQ( 0b11100111, data[0]);
    EXPECT_EQ( 0b01100100, data[1]);
    EXPECT_EQ( 0b10000111, data[2]);
    EXPECT_EQ( 0b00110110, data[3]);
    EXPECT_EQ( 0b00110000, data[4]);
}
// +---------+---------+---------+---------+---------+---------+---------+---------+---------+
// |0110|1101 0101 0010 0100 0011 1000 1101 1000 0110 0101 0010 0100 0011 1000 1101 1000|0110|
// +---------+---------+---------+---------+---------+---------+---------+---------+---------+
TEST_F(BitstreamTest, put64Bit)
{
    uint8_t data[9]; 
    BitstreamWriter bs(data, 9);
    bs.put<4>(0b0110);
    bs.put<64>(0b1101010100100100001110001101100001100101001001000011100011011000);
    bs.put<4>(0b0110);

    EXPECT_EQ( 0b01101101, data[0]);
    EXPECT_EQ( 0b01010010, data[1]);
    EXPECT_EQ( 0b01000011, data[2]);
    EXPECT_EQ( 0b10001101, data[3]);
    EXPECT_EQ( 0b10000110, data[4]);
    EXPECT_EQ( 0b01010010, data[5]);
    EXPECT_EQ( 0b01000011, data[6]);
    EXPECT_EQ( 0b10001101, data[7]);
    EXPECT_EQ( 0b10000110, data[8]);
}
TEST_F(BitstreamTest, putAndGetFun)
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

    EXPECT_EQ(bsr.get<4>(), a);
    EXPECT_EQ(bsr.get<64>(), b);
    EXPECT_EQ(bsr.get<5>(), c);
    EXPECT_EQ(bsr.get<10>(), d);
    EXPECT_EQ(bsr.get<17>(), e);
}

// TEST_F(BitstreamTest, putMoreThanWriterCanTake)
