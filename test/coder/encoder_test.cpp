#include "coder/defs.hpp"
#include "coder/encode_exception.hpp"
#include "coder/encoder.hpp"
#include "coder/util.hpp"
#include <gtest/gtest.h>

namespace fict_tele
{

class EncoderTestSuite : public testing::Test
{
  public:
    EncoderTestSuite()
        : data(64), span{data.begin(), data.end()}, iterator{span.begin()}, filename{"x.png"}, bitsPerChannel{}
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

    std::vector<Byte> data;
    std::span<Byte> span;
    std::span<Byte>::iterator iterator;

    std::string filename;
    int bitsPerChannel;
};

TEST_F(EncoderTestSuite, Encode2Bits)
{
    const std::vector<Byte> message{'x', 'y', 'z'};
    bitsPerChannel = 2;

    Encoder encoder{span, bitsPerChannel};
    encoder.encode(message, filename);

    // Encoded layout:
    // Filename length = 4 bytes (encoded on 32 bits) = 0b00000000'00000000'00000000'00000101
    // Filename = 01111000 00101110 01110000 01101110 01100111
    // Message length = 4 bytes (encoded on 32 bits) = 0b00000000'00000000'00000000'00000011
    // Message bytes [...]

    // Using 2 bits per channel.
    // Thus first 14 channels are = 0
    for (int i = 0; i < 14; i++)
    {
        EXPECT_EQ(*iterator++, Byte{0b0000'0000});
    }
    // 15th/16th channel has '0101' encoded = filename length
    EXPECT_EQ(*iterator++, Byte{0b0000'0001});
    EXPECT_EQ(*iterator++, Byte{0b0000'0001});

    // x = 0111 1000
    EXPECT_EQ(*iterator++, Byte{0b0000'0001});
    EXPECT_EQ(*iterator++, Byte{0b0000'0011});
    EXPECT_EQ(*iterator++, Byte{0b0000'0010});
    EXPECT_EQ(*iterator++, Byte{0b0000'0000});

    // . = 0010 1110
    EXPECT_EQ(*iterator++, Byte{0b0000'0000});
    EXPECT_EQ(*iterator++, Byte{0b0000'0010});
    EXPECT_EQ(*iterator++, Byte{0b0000'0011});
    EXPECT_EQ(*iterator++, Byte{0b0000'0010});

    // p = 0111 0000
    EXPECT_EQ(*iterator++, Byte{0b0000'0001});
    EXPECT_EQ(*iterator++, Byte{0b0000'0011});
    EXPECT_EQ(*iterator++, Byte{0b0000'0000});
    EXPECT_EQ(*iterator++, Byte{0b0000'0000});

    // n = 0110 1110
    EXPECT_EQ(*iterator++, Byte{0b0000'0001});
    EXPECT_EQ(*iterator++, Byte{0b0000'0010});
    EXPECT_EQ(*iterator++, Byte{0b0000'0011});
    EXPECT_EQ(*iterator++, Byte{0b0000'0010});

    // g = 0110 0111
    EXPECT_EQ(*iterator++, Byte{0b0000'0001});
    EXPECT_EQ(*iterator++, Byte{0b0000'0010});
    EXPECT_EQ(*iterator++, Byte{0b0000'0001});
    EXPECT_EQ(*iterator++, Byte{0b0000'0011});

    // Message Length
    for (int i = 0; i < 15; i++)
    {
        EXPECT_EQ(*iterator++, Byte{0b0000'0000});
    }
    EXPECT_EQ(*iterator++, Byte{0b0000'0011});

    // Data
    verifyMessageEncoded(message);
}

TEST_F(EncoderTestSuite, Encode4Bits)
{
    const std::vector<Byte> message{'w', 'o', 'l', 'f'};
    bitsPerChannel = 4;

    Encoder encoder{span, bitsPerChannel};
    encoder.encode(message, filename);

    // Encoded layout:
    // Filename length = 4 bytes (encoded on 32 bits) = 0b00000000'00000000'00000000'00000101
    // Filename = 01111000 00101110 01110000 01101110 01100111
    // Message length = 4 bytes (encoded on 32 bits) = 0b00000000'00000000'00000000'00000100
    // Message bytes [...]

    // Using 4 bits per channel.
    // Thus first 7 channels are = 0
    for (int i = 0; i < 7; i++)
    {
        EXPECT_EQ(*iterator++, Byte{0b0000'0000});
    }

    // 8th channel has '0101' encoded = filename length
    EXPECT_EQ(*iterator++, Byte{0b0000'0101});

    // x
    EXPECT_EQ(*iterator++, Byte{0b0000'0111});
    EXPECT_EQ(*iterator++, Byte{0b0000'1000});

    // .
    EXPECT_EQ(*iterator++, Byte{0b0000'0010});
    EXPECT_EQ(*iterator++, Byte{0b0000'1110});

    // p
    EXPECT_EQ(*iterator++, Byte{0b0000'0111});
    EXPECT_EQ(*iterator++, Byte{0b0000'0000});

    // n
    EXPECT_EQ(*iterator++, Byte{0b0000'0110});
    EXPECT_EQ(*iterator++, Byte{0b0000'1110});

    // g
    EXPECT_EQ(*iterator++, Byte{0b0000'0110});
    EXPECT_EQ(*iterator++, Byte{0b0000'0111});

    // Data Length
    for (int i = 0; i < 7; i++)
    {
        EXPECT_EQ(*iterator++, Byte{0b0000'0000});
    }
    EXPECT_EQ(*iterator++, Byte{0b0000'0100});

    // Data
    verifyMessageEncoded(message);
}

TEST_F(EncoderTestSuite, TooLongMessage)
{
    bitsPerChannel = 2;
    const auto maxBytesToEncode = (span.size() * bitsPerChannel) / BITS_IN_BYTE;
    const auto maxMessageSize = maxBytesToEncode - 2 * NUM_BYTES_LENGTH_ENCODED - filename.size();

    {
        const std::vector<Byte> message(maxMessageSize);
        Encoder encoder{span, bitsPerChannel};
        EXPECT_NO_THROW(encoder.encode(message, filename));
    }

    {
        const std::vector<Byte> message(maxMessageSize + 1);
        Encoder encoder{span, bitsPerChannel};
        EXPECT_THROW(encoder.encode(message, filename), EncodeException);
    }
}

}
