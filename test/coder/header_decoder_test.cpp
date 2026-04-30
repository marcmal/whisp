#include <gtest/gtest.h>

import whisp.util;
import whisp.coder;

namespace whisp
{
using util::Byte;

class HeaderDecoderTest : public testing::Test
{
  public:
    HeaderDecoderTest() : data{}, span{}, iterator{}
    {
    }

  protected:
    auto decode() const
    {
        return coder::decode(span);
    }

    void fillEncoded(const std::vector<Byte>& bytes)
    {
        for (const auto byte : bytes)
        {
            *iterator = byte;
            iterator += 4;
        }
    }

    void prepareBuffer(const std::size_t size)
    {
        data.resize(size);
        span = std::span{data};
        iterator = span.begin() + 3;
    }

    std::vector<Byte> data;
    std::span<Byte> span;
    std::span<Byte>::iterator iterator;
};

TEST_F(HeaderDecoderTest, UnknownAlgorithmMode)
{
    constexpr auto bufferSize = 12;
    prepareBuffer(bufferSize);

    constexpr auto unknownMode = 3;
    fillEncoded({unknownMode});

    const auto result = decode();
    EXPECT_FALSE(result.has_value());

    const auto error = result.error();
    EXPECT_STREQ(error.what(), "Decoding algorithm header failure, mode: 3");
}

}
