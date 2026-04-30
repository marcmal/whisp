#include <gtest/gtest.h>

import whisp.util;
import whisp.coder;

namespace whisp
{
using util::Byte;

namespace
{
constexpr auto ALPHA_MODE = 1;
constexpr auto ALPHA_HEADER_SIZE = 4;
constexpr auto NUM_BYTES_LENGTH_ENCODED = 4;

auto calculateMinBufferSize(const std::optional<std::size_t> filenameSize, const std::optional<std::size_t> dataSize)
{
    std::size_t size{};

    if (filenameSize.has_value())
    {
        size += NUM_BYTES_LENGTH_ENCODED;
        size += filenameSize.value();
    }

    if (dataSize.has_value())
    {
        size += NUM_BYTES_LENGTH_ENCODED;
        size += dataSize.value();
    }

    return size * 4 + ALPHA_HEADER_SIZE;
}
}

class AlphaDecoderTestSuite : public testing::Test
{
  public:
    AlphaDecoderTestSuite() : data{}, span{}, iterator{}
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

TEST_F(AlphaDecoderTestSuite, NotEnoughDataToDecodeFilenameLength)
{
    prepareBuffer(calculateMinBufferSize({}, {}));

    // Header
    fillEncoded({ALPHA_MODE});

    const auto decoded = decode();
    EXPECT_FALSE(decoded.has_value());

    const auto error = decoded.error();
    EXPECT_STREQ(error.what(), "Not enough data to decode.");
}

TEST_F(AlphaDecoderTestSuite, NotEnoughDataToDecodeFilename)
{
    const auto filenameLength = 10;
    prepareBuffer(calculateMinBufferSize(filenameLength, {}) - 1);

    // Header
    fillEncoded({ALPHA_MODE});

    fillEncoded(std::vector<Byte>(3, Byte{}));
    fillEncoded({Byte{filenameLength}});

    auto decoded = decode();
    EXPECT_FALSE(decoded.has_value());
}

TEST_F(AlphaDecoderTestSuite, NotEnoughDataToDecodeDataLength)
{
    const auto filenameLength = 12;
    prepareBuffer(calculateMinBufferSize(filenameLength, {0}) - 1);

    // Header
    fillEncoded({ALPHA_MODE});

    fillEncoded({0, 0, 0, static_cast<Byte>(filenameLength)});

    auto decoded = decode();
    EXPECT_FALSE(decoded.has_value());
}

TEST_F(AlphaDecoderTestSuite, NotEnoughDataToDecodeData)
{
    const auto filenameLength = 3;
    const auto dataLength = 13;

    prepareBuffer(calculateMinBufferSize(filenameLength, dataLength) - 1);

    // Header
    fillEncoded({ALPHA_MODE});

    fillEncoded({0, 0, 0, static_cast<Byte>(filenameLength)});
    fillEncoded({1, 2, 3});

    fillEncoded({0, 0, 0, static_cast<Byte>(dataLength)});

    auto decoded = decode();
    EXPECT_FALSE(decoded.has_value());
}

TEST_F(AlphaDecoderTestSuite, Decode)
{
    const auto expectedData = std::vector<Byte>{'F', 'o', 'x'};
    const auto expectedFilename = std::string{"a.txt"};

    prepareBuffer(calculateMinBufferSize(expectedFilename.size(), expectedData.size()));

    // Header
    fillEncoded({ALPHA_MODE});

    // Filename => a.txt = length = 4 (0b...0101)
    fillEncoded({0, 0, 0, static_cast<Byte>(expectedFilename.size())});
    fillEncoded({static_cast<Byte>('a')});
    fillEncoded({static_cast<Byte>('.')});
    fillEncoded({static_cast<Byte>('t')});
    fillEncoded({static_cast<Byte>('x')});
    fillEncoded({static_cast<Byte>('t')});

    fillEncoded({0, 0, 0, static_cast<Byte>(expectedData.size())});
    fillEncoded({static_cast<Byte>('F')});
    fillEncoded({static_cast<Byte>('o')});
    fillEncoded({static_cast<Byte>('x')});

    auto decoded = decode();
    ASSERT_TRUE(decoded.has_value());

    const auto [data, filename] = decoded.value();
    EXPECT_EQ(filename, expectedFilename);
    EXPECT_EQ(data, expectedData);
}

}
