#include <gtest/gtest.h>

import whisp.util;
import whisp.coder;
import whisp.parser;

namespace whisp
{
using util::Byte;

namespace
{
constexpr auto RGB_HEADER_SIZE = 2;
constexpr auto RGB_MODE = 2;
constexpr auto BITS_IN_BYTE = 8;
constexpr auto NUM_BYTES_LENGTH_ENCODED = 4;
}

class RgbEncoderTestSuite : public testing::Test
{
  public:
    RgbEncoderTestSuite() : data(128), span{data}, iterator{span.begin()}, filename{"x.png"}, bitsPerChannel{}
    {
    }

  protected:
    auto encode(const auto& dataBuffer) const
    {
        algorithm::RgbConfig config{bitsPerChannel};
        coder::CoderData dataToEncode{dataBuffer, filename};
        return coder::encode(config, span, dataToEncode);
    }

    void checkByte(Byte byte)
    {
        for (auto shift = BITS_IN_BYTE - bitsPerChannel; shift >= 0; shift -= bitsPerChannel)
        {
            EXPECT_EQ((byte >> shift) & mask, *iterator++);
        }
    }

    void verifyBytesEncoded(const std::vector<Byte>& expected)
    {
        for (const auto byte : expected)
        {
            EXPECT_EQ(*iterator++, byte);
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
    Byte mask;
};

TEST_F(RgbEncoderTestSuite, Encode2Bits)
{
    const std::vector<Byte> message{'x', 'y', 'z'};
    bitsPerChannel = 2;
    mask = 0b0000'0011;

    encode(message);

    // Header
    verifyBytesEncoded({0, 0, 0, RGB_MODE});
    verifyBytesEncoded({0, 0, 0, static_cast<Byte>(bitsPerChannel)});

    // Encoded layout:
    // Filename length = 4 bytes (encoded on 32 bits) = 0b00000000'00000000'00000000'00000101
    // Filename = 01111000 00101110 01110000 01101110 01100111
    // Message length = 4 bytes (encoded on 32 bits) = 0b00000000'00000000'00000000'00000011
    // Message bytes [...]

    // Using 2 bits per channel.
    // Thus first 14 channels are = 0

    // Filename length
    verifyBytesEncoded(std::vector<Byte>(14, Byte{0b0000'0000}));
    verifyBytesEncoded({Byte{0b0000'0001}, Byte{0b0000'0001}});

    // x = 0111 1000
    verifyBytesEncoded({Byte{0b0000'0001}, Byte{0b0000'0011}, Byte{0b0000'0010}, Byte{0b0000'0000}});

    // . = 0010 1110
    verifyBytesEncoded({Byte{0b0000'0000}, Byte{0b0000'0010}, Byte{0b0000'0011}, Byte{0b0000'0010}});

    // p = 0111 0000
    verifyBytesEncoded({Byte{0b0000'0001}, Byte{0b0000'0011}, Byte{0b0000'0000}, Byte{0b0000'0000}});

    // n = 0110 1110
    verifyBytesEncoded({Byte{0b0000'0001}, Byte{0b0000'0010}, Byte{0b0000'0011}, Byte{0b0000'0010}});

    // g = 0110 0111
    verifyBytesEncoded({Byte{0b0000'0001}, Byte{0b0000'0010}, Byte{0b0000'0001}, Byte{0b0000'0011}});

    // Message Length
    verifyBytesEncoded(std::vector<Byte>(15, Byte{0b0000'0000}));
    verifyBytesEncoded({Byte{0b0000'0011}});

    // Data
    verifyMessageEncoded(message);
}

TEST_F(RgbEncoderTestSuite, Encode4Bits)
{
    const std::vector<Byte> message{'w', 'o', 'l', 'f'};
    bitsPerChannel = 4;
    mask = 0b0000'1111;

    encode(message);

    // Header
    verifyBytesEncoded({0, 0, 0, RGB_MODE});
    verifyBytesEncoded({0, 0, 0, static_cast<Byte>(bitsPerChannel)});

    // Encoded layout:
    // Filename length = 4 bytes (encoded on 32 bits) = 0b00000000'00000000'00000000'00000101
    // Filename = 01111000 00101110 01110000 01101110 01100111
    // Message length = 4 bytes (encoded on 32 bits) = 0b00000000'00000000'00000000'00000100
    // Message bytes [...]

    // Using 4 bits per channel.
    // Thus first 7 channels are = 0

    // Filename length
    verifyBytesEncoded(std::vector<Byte>(7, Byte{0b0000'0000}));
    EXPECT_EQ(*iterator++, Byte{0b0000'0101});

    // x
    verifyBytesEncoded({Byte{0b0000'0111}, Byte{0b0000'1000}});

    // .
    verifyBytesEncoded({Byte{0b0000'0010}, Byte{0b0000'1110}});

    // p
    verifyBytesEncoded({Byte{0b0000'0111}, Byte{0b0000'0000}});

    // n
    verifyBytesEncoded({Byte{0b0000'0110}, Byte{0b0000'1110}});

    // g
    verifyBytesEncoded({Byte{0b0000'0110}, Byte{0b0000'0111}});

    // Data Length
    verifyBytesEncoded(std::vector<Byte>(7, Byte{0b0000'0000}));
    EXPECT_EQ(*iterator++, Byte{0b0000'0100});

    // Data
    verifyMessageEncoded(message);
}

TEST_F(RgbEncoderTestSuite, NotEnoughBufferToEncoderHeader)
{
    data.resize(7);
    span = std::span{data};

    const auto result = encode(std::vector<Byte>{});
    ASSERT_FALSE(result.has_value());
    EXPECT_STREQ(result.error().what(), "Not enough buffer to encode header.");
}

TEST_F(RgbEncoderTestSuite, NotEnoughBufferToEncodeData)
{
    bitsPerChannel = 2;
    mask = 0b0000'0011;

    const auto maxBytesToEncode = (span.size() * bitsPerChannel) / BITS_IN_BYTE;
    const auto maxMessageSize = maxBytesToEncode - 2 * NUM_BYTES_LENGTH_ENCODED - filename.size() - RGB_HEADER_SIZE;

    {
        const std::vector<Byte> message(maxMessageSize);
        const auto result = encode(message);
        EXPECT_TRUE(result.has_value());
    }

    {
        const std::vector<Byte> message(maxMessageSize + 1);
        const auto result = encode(message);
        ASSERT_FALSE(result.has_value());

        const auto error = result.error();
        EXPECT_STREQ(error.what(), "Too long message to encode.");
    }
}

}
