#include <gtest/gtest.h>

import whisp.util;
import whisp.coder;
import whisp.parser;

namespace whisp
{

using util::Byte;

namespace
{
constexpr auto ALPHA_HEADER_SIZE = 1;
constexpr auto ALPHA_MODE = 1;
}

class AlphaEncoderTestSuite : public testing::Test
{
  public:
    AlphaEncoderTestSuite() : data(128), span{data}, iterator{span.begin()}, filename{"x.png"}
    {
        iterator += 3;
    }

  protected:
    auto encode(const auto& dataBuffer) const
    {
        algorithm::AlphaConfig config{};
        coder::CoderData dataToEncode{dataBuffer, filename};
        return coder::encode(config, span, dataToEncode);
    }

    void checkByte(Byte byte)
    {
        EXPECT_EQ(byte, *iterator);
        iterator += 4;
    }

    void verifyBytesEncoded(const std::vector<Byte>& expected)
    {
        for (const auto byte : expected)
        {
            checkByte(byte);
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
};

TEST_F(AlphaEncoderTestSuite, Encode)
{
    const std::vector<Byte> message{'x', 'y', 'z'};

    encode(message);

    // Header
    verifyBytesEncoded({ALPHA_MODE});

    // Filename length
    verifyBytesEncoded({0, 0, 0, static_cast<Byte>(filename.size())});

    verifyBytesEncoded({static_cast<Byte>('x')});
    verifyBytesEncoded({static_cast<Byte>('.')});
    verifyBytesEncoded({static_cast<Byte>('p')});
    verifyBytesEncoded({static_cast<Byte>('n')});
    verifyBytesEncoded({static_cast<Byte>('g')});

    // Message length
    verifyBytesEncoded({{0, 0, 0, static_cast<Byte>(message.size())}});

    // Data
    verifyMessageEncoded(message);
}

TEST_F(AlphaEncoderTestSuite, NotEnoughBufferToEncoderHeader)
{
    data.resize(3);
    span = std::span{data};

    const auto result = encode(std::vector<Byte>{});
    ASSERT_FALSE(result.has_value());
    EXPECT_STREQ(result.error().what(), "Not enough buffer to encode header.");
}

TEST_F(AlphaEncoderTestSuite, NotEnoughBufferToEncodeData)
{
    const auto maxBytesToEncode = span.size() / 4;
    const auto maxMessageSize = maxBytesToEncode - 4 - 4 - filename.size() - ALPHA_HEADER_SIZE;

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
