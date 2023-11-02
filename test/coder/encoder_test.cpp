#include "coder/inc/defs.hpp"
#include "coder/inc/encode_exception.hpp"
#include "coder/inc/encoder.hpp"
#include "coder/inc/util.hpp"
#include <CImg.h>
#include <gtest/gtest.h>

namespace stegan
{

class EncoderTestSuite : public testing::Test
{
  public:
    EncoderTestSuite() : image(4, 4, 1, 3, {}), iterator{image}, bitsPerChannel{}
    {
    }

  protected:
    void checkByte(Byte byte)
    {
        const auto mask = createMask(bitsPerChannel);
        for (auto shift = BITS_IN_BYTE - bitsPerChannel; shift >= 0; shift -= bitsPerChannel)
        {
            EXPECT_EQ((byte >> shift) & mask, *iterator++);
        }
    }

    void verifyMessageEncoded(const std::vector<Byte>& content)
    {
        for (const auto& byte : content)
        {
            checkByte(byte);
        }
    }

    cimg_library::CImg<Byte> image;
    std::span<Byte> span{image.begin(), image.end()};
    std::span<Byte>::iterator iterator;
    int bitsPerChannel;
};

TEST_F(EncoderTestSuite, Encode2Bits)
{
    const std::vector<Byte> message{'x', 'y', 'z'};
    bitsPerChannel = 2;

    Encoder encoder{image, bitsPerChannel};
    encoder.encode(message);

    // Message length = 3 bytes (encoded on 32 bits) = 0b00000000'00000000'00000000'00000011
    // Using 2 bits per channel.
    // Thus first 15 channels are = 0
    // 16th channel has '11' encoded

    for (int i = 0; i < 15; i++)
    {
        EXPECT_EQ(*iterator++, (Byte){0b0000'0000});
    }

    // Message Size
    EXPECT_EQ(*iterator++, (Byte){0b0000'0011});
    verifyMessageEncoded(message);
}

TEST_F(EncoderTestSuite, Encode4Bits)
{
    const std::vector<Byte> message{'w', 'o', 'l', 'f'};
    bitsPerChannel = 4;

    Encoder encoder{image, bitsPerChannel};
    encoder.encode(message);

    // Message length = 4 bytes (encoded on 32 bits) = 0b00000000'00000000'00000000'00000100
    // Using 4 bits per channel.
    // Thus first 7 channels are = 0
    // 8th channel has '0100' encoded

    for (int i = 0; i < 7; i++)
    {
        EXPECT_EQ(*iterator++, (Byte){0b0000'0000});
    }

    // Message Size
    EXPECT_EQ(*iterator++, (Byte){0b0000'0100});
    verifyMessageEncoded(message);
}

TEST_F(EncoderTestSuite, TooLongMessage)
{
    const std::vector<Byte> message(30);
    bitsPerChannel = 2;

    Encoder encoder{image, bitsPerChannel};
    EXPECT_THROW(encoder.encode(message), EncodeException);
}

}
