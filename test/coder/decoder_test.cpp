#include "coder/decoder.hpp"
#include "coder/defs.hpp"
#include "coder/util.hpp"
#include <gtest/gtest.h>

namespace fict_tele
{

class DecoderTestSuite : public testing::Test
{
  public:
    DecoderTestSuite() : data(64), span{data.begin(), data.end()}, iterator{span.begin()}, bitsPerChannel{}
    {
    }

  protected:
    std::vector<Byte> data;
    std::span<Byte> span;
    std::span<Byte>::iterator iterator;
    int bitsPerChannel;
};

TEST_F(DecoderTestSuite, Decode2Bits)
{
    bitsPerChannel = 2;

    // Filename => a.txt = length = 4 (0b...0101)
    for (int i = 0; i < 14; i++)
    {
        *iterator++ = Byte{0b0000'0000};
    }
    *iterator++ = Byte{0b0000'0001};
    *iterator++ = Byte{0b0000'0001};

    // a = 0110 0001
    *iterator++ = Byte{0b0000'0001};
    *iterator++ = Byte{0b0000'0010};
    *iterator++ = Byte{0b0000'0000};
    *iterator++ = Byte{0b0000'0001};

    // . = 0010 1110
    *iterator++ = Byte{0b0000'0000};
    *iterator++ = Byte{0b0000'0010};
    *iterator++ = Byte{0b0000'0011};
    *iterator++ = Byte{0b0000'0010};

    // t = 0111 0100
    *iterator++ = Byte{0b0000'0001};
    *iterator++ = Byte{0b0000'0011};
    *iterator++ = Byte{0b0000'0001};
    *iterator++ = Byte{0b0000'0000};

    // x = 0111 1000
    *iterator++ = Byte{0b0000'0001};
    *iterator++ = Byte{0b0000'0011};
    *iterator++ = Byte{0b0000'0010};
    *iterator++ = Byte{0b0000'0000};

    // t = 0111 0100
    *iterator++ = Byte{0b0000'0001};
    *iterator++ = Byte{0b0000'0011};
    *iterator++ = Byte{0b0000'0001};
    *iterator++ = Byte{0b0000'0000};

    // Data length = 3 (0b...0011)
    for (int i = 0; i < 15; i++)
    {
        *iterator++ = Byte{0b0000'0000};
    }
    *iterator++ = Byte{0b0000'0011};

    // F = 0100 0110
    *iterator++ = Byte{0b0000'0001};
    *iterator++ = Byte{0b0000'0000};
    *iterator++ = Byte{0b0000'0001};
    *iterator++ = Byte{0b0000'0010};

    // o = 0110 1111
    *iterator++ = Byte{0b0000'0001};
    *iterator++ = Byte{0b0000'0010};
    *iterator++ = Byte{0b0000'0011};
    *iterator++ = Byte{0b0000'0011};

    // x = 0111 1000
    *iterator++ = Byte{0b0000'0001};
    *iterator++ = Byte{0b0000'0011};
    *iterator++ = Byte{0b0000'0010};
    *iterator++ = Byte{0b0000'0000};

    const auto expectedData = std::vector<Byte>{'F', 'o', 'x'};
    const auto expectedFilename = std::string{"a.txt"};
    const auto [filename, data] = Decoder{span, bitsPerChannel}.decode();
    EXPECT_EQ(filename, expectedFilename);
    EXPECT_EQ(data, expectedData);
}

TEST_F(DecoderTestSuite, Decode4Bits)
{
    bitsPerChannel = 4;

    // Filename => mx.mp3 = length = 6 (0b...0110)
    for (int i = 0; i < 7; i++)
    {
        *iterator++ = Byte{0b0000'0000};
    }
    *iterator++ = Byte{0b0000'0110};

    // m
    *iterator++ = Byte{0b0000'0110};
    *iterator++ = Byte{0b0000'1101};

    // x
    *iterator++ = Byte{0b0000'0111};
    *iterator++ = Byte{0b0000'1000};

    // .
    *iterator++ = Byte{0b0000'0010};
    *iterator++ = Byte{0b0000'1110};

    // m
    *iterator++ = Byte{0b0000'0110};
    *iterator++ = Byte{0b0000'1101};

    // p
    *iterator++ = Byte{0b0000'0111};
    *iterator++ = Byte{0b0000'0000};

    // 3
    *iterator++ = Byte{0b0000'0011};
    *iterator++ = Byte{0b0000'0011};

    // Data length = 4 (0b...0100)
    for (int i = 0; i < 7; i++)
    {
        *iterator++ = Byte{0b0000'0000};
    }
    *iterator++ = Byte{0b0000'0100};

    // L = 0100 1100
    *iterator++ = Byte{0b0000'0100};
    *iterator++ = Byte{0b0000'1100};

    // y = 0111 1001
    *iterator++ = Byte{0b0000'0111};
    *iterator++ = Byte{0b0000'1001};

    // n = 0110 1110
    *iterator++ = Byte{0b0000'0110};
    *iterator++ = Byte{0b0000'1110};

    // x = 0111 1000
    *iterator++ = Byte{0b0000'0111};
    *iterator++ = Byte{0b0000'1000};

    const auto expectedData = std::vector<Byte>{'L', 'y', 'n', 'x'};
    const auto expectedFilename = std::string{"mx.mp3"};
    const auto [filename, data] = Decoder{span, bitsPerChannel}.decode();
    EXPECT_EQ(filename, expectedFilename);
    EXPECT_EQ(data, expectedData);
}

}
