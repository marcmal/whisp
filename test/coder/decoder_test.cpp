#include "coder/decoder.hpp"
#include "coder/defs.hpp"
#include "coder/util.hpp"
#include <gtest/gtest.h>

namespace fict_tele
{

class DecoderTestSuite : public testing::Test
{
  public:
    DecoderTestSuite()
        : data(32), span{data.begin(), data.end()}, iterator{span.begin()}, bitsPerChannel{}
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

    const auto expected = std::vector<Byte>{'F', 'o', 'x'};
    const auto decoded = Decoder{span, bitsPerChannel}.decode();
    EXPECT_EQ(decoded, expected);
}

TEST_F(DecoderTestSuite, Decode4Bits)
{
    bitsPerChannel = 4;

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

    const auto expected = std::vector<Byte>{'L', 'y', 'n', 'x'};
    const auto decoded = Decoder{span, bitsPerChannel}.decode();
    EXPECT_EQ(decoded, expected);
}

}
