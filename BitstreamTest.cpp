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
    Bitstream bs(data, 1);
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
    Bitstream bs(data, 1);
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
    Bitstream bs(data, 1);
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
    Bitstream bs(data, 2);
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
    Bitstream bs(data, 2);
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
    Bitstream bs(data, 4);
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
    Bitstream bs(data, 8);
    uint8_t a = bs.get<3>();
    EXPECT_EQ(0b000, a);
    uint8_t b = bs.get<3>();
    EXPECT_EQ(0b111, b);
    uint8_t c = bs.get<3>();
    EXPECT_EQ(0b000, c);
    uint8_t d = bs.get<3>();
    EXPECT_EQ(0b111, d);
    uint8_t e = bs.get<3>();
    EXPECT_EQ(0b000, e);
    uint8_t f = bs.get<3>();
    EXPECT_EQ(0b111, f);
    uint8_t g = bs.get<3>();
    EXPECT_EQ(0b000, g);
    uint8_t h = bs.get<3>();
    EXPECT_EQ(0b111, h);
    uint8_t i = bs.get<3>();
    EXPECT_EQ(0b000, i);
    uint8_t j = bs.get<3>();
    EXPECT_EQ(0b111, j);
    uint32_t k = bs.get<30>();
    EXPECT_EQ(0b000001110001110001110001110001, k);
    uint8_t l = bs.get<4>();
    EXPECT_EQ(0b1100, l);
}
