#include "coder/decode.hpp"
#include "coder/error.hpp"
#include "util/types.hpp"
#include <gtest/gtest.h>

namespace whisp
{
namespace
{
constexpr auto RGB_MODE = 2;
constexpr auto RGB_HEADER_SIZE = 2 * 4;
constexpr auto BITS_IN_BYTE = 8;
constexpr auto NUM_BYTES_LENGTH_ENCODED = 4;

auto calculateMinBufferSize(const std::optional<std::size_t> filenameSize,
                            const std::optional<std::size_t> dataSize,
                            const int bitsPerChannel)
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

    return size * BITS_IN_BYTE / bitsPerChannel + RGB_HEADER_SIZE;
}
}

class RgbDecoderTestSuite : public testing::Test
{
  public:
    RgbDecoderTestSuite() : data{}, span{data}, iterator{span.begin()}, bitsPerChannel{}
    {
    }

  protected:
    auto decode() const
    {
        return whisp::decode(span);
    }

    void fillEncoded(const std::vector<Byte>& bytes)
    {
        for (const auto byte : bytes)
        {
            *iterator++ = byte;
        }
    }

    void prepareBuffer(const std::size_t size)
    {
        data.resize(size);
        span = std::span{data};
        iterator = span.begin();
    }

    std::vector<Byte> data;
    std::span<Byte> span;
    std::span<Byte>::iterator iterator;
    int bitsPerChannel;
};

TEST_F(RgbDecoderTestSuite, NotEnoughDataToDecodeFilenameLength)
{
    bitsPerChannel = 4;
    prepareBuffer(calculateMinBufferSize({}, {}, bitsPerChannel));

    // Header
    fillEncoded({0, 0, 0, RGB_MODE});
    fillEncoded({0, 0, 0, static_cast<Byte>(bitsPerChannel)});

    const auto decoded = decode();
    EXPECT_FALSE(decoded.has_value());

    const auto error = decoded.error();
    EXPECT_STREQ(error.what(), "Not enough data to decode.");
}

TEST_F(RgbDecoderTestSuite, NotEnoughDataToDecodeFilename)
{
    bitsPerChannel = 4;
    const auto filenameLength = 10;
    prepareBuffer(calculateMinBufferSize(filenameLength, {}, bitsPerChannel) - 1);

    // Header
    fillEncoded({0, 0, 0, RGB_MODE});
    fillEncoded({0, 0, 0, static_cast<Byte>(bitsPerChannel)});

    fillEncoded(std::vector<Byte>(7, Byte{}));
    fillEncoded({Byte{filenameLength}});

    auto decoded = decode();
    EXPECT_FALSE(decoded.has_value());
}

TEST_F(RgbDecoderTestSuite, NotEnoughDataToDecodeDataLength)
{
    bitsPerChannel = 2;
    const auto filenameLength = 12;
    prepareBuffer(calculateMinBufferSize(filenameLength, {0}, bitsPerChannel) - 1);

    // Header
    fillEncoded({0, 0, 0, RGB_MODE});
    fillEncoded({0, 0, 0, static_cast<Byte>(bitsPerChannel)});

    fillEncoded(std::vector<Byte>(14, Byte{}));
    fillEncoded({Byte{0b0000'0011}, Byte{0b0000'0000}});

    auto decoded = decode();
    EXPECT_FALSE(decoded.has_value());
}

TEST_F(RgbDecoderTestSuite, NotEnoughDataToDecodeData)
{
    bitsPerChannel = 4;
    const auto filenameLength = 3;
    const auto dataLength = 13;

    prepareBuffer(calculateMinBufferSize(filenameLength, dataLength, bitsPerChannel) - 1);

    // Header
    fillEncoded({0, 0, 0, RGB_MODE});
    fillEncoded({0, 0, 0, static_cast<Byte>(bitsPerChannel)});

    fillEncoded(std::vector<Byte>(7, Byte{}));
    fillEncoded({filenameLength});
    fillEncoded({0b0000'0111, 0b0000'0000, 0b0000'0110, 0b0000'1110, 0b0000'0110, 0b0000'0111});

    fillEncoded(std::vector<Byte>(7, Byte{}));
    fillEncoded({dataLength});

    auto decoded = decode();
    EXPECT_FALSE(decoded.has_value());
}

TEST_F(RgbDecoderTestSuite, Decode2Bits)
{
    const auto expectedData = std::vector<Byte>{'F', 'o', 'x'};
    const auto expectedFilename = std::string{"a.txt"};

    bitsPerChannel = 2;
    prepareBuffer(calculateMinBufferSize(expectedFilename.size(), expectedData.size(), bitsPerChannel));

    // Header
    fillEncoded({0, 0, 0, RGB_MODE});
    fillEncoded({0, 0, 0, static_cast<Byte>(bitsPerChannel)});

    // Filename => a.txt = length = 4 (0b...0101)
    fillEncoded(std::vector<Byte>(14, Byte{}));
    fillEncoded({0b0000'0001, 0b0000'0001});

    // a = 0110 0001
    fillEncoded({0b0000'0001, 0b0000'0010, 0b0000'0000, 0b0000'0001});

    // . = 0010 1110
    fillEncoded({0b0000'0000, 0b0000'0010, 0b0000'0011, 0b0000'0010});

    // t = 0111 0100
    fillEncoded({0b0000'0001, 0b0000'0011, 0b0000'0001, 0b0000'0000});

    // x = 0111 1000
    fillEncoded({0b0000'0001, 0b0000'0011, 0b0000'0010, 0b0000'0000});

    // t = 0111 0100
    fillEncoded({0b0000'0001, 0b0000'0011, 0b0000'0001, 0b0000'0000});

    // Data length = 3 (0b...0011)
    fillEncoded(std::vector<Byte>(15, Byte{}));
    fillEncoded({0b0000'0011});

    // F = 0100 0110
    fillEncoded({0b0000'0001, 0b0000'0000, 0b0000'0001, 0b0000'0010});

    // o = 0110 1111
    fillEncoded({0b0000'0001, 0b0000'0010, 0b0000'0011, 0b0000'0011});

    // x = 0111 1000
    fillEncoded({0b0000'0001, 0b0000'0011, 0b0000'0010, 0b0000'0000});

    auto decoded = decode();
    ASSERT_TRUE(decoded.has_value());

    const auto [data, filename] = decoded.value();
    EXPECT_EQ(filename, expectedFilename);
    EXPECT_EQ(data, expectedData);
}

TEST_F(RgbDecoderTestSuite, Decode4Bits)
{
    const auto expectedData = std::vector<Byte>{'L', 'y', 'n', 'x'};
    const auto expectedFilename = std::string{"mx.mp3"};

    bitsPerChannel = 4;
    prepareBuffer(calculateMinBufferSize(expectedFilename.size(), expectedData.size(), bitsPerChannel));

    // Header
    fillEncoded({0, 0, 0, RGB_MODE});
    fillEncoded({0, 0, 0, static_cast<Byte>(bitsPerChannel)});

    // Filename => mx.mp3 = length = 6 (0b...0110)
    fillEncoded(std::vector<Byte>(7, Byte{}));
    fillEncoded({0b0000'0110});

    // m
    fillEncoded({0b0000'0110, 0b0000'1101});

    // x
    fillEncoded({0b0000'0111, 0b0000'1000});

    // .
    fillEncoded({0b0000'0010, 0b0000'1110});

    // m
    fillEncoded({0b0000'0110, 0b0000'1101});

    // p
    fillEncoded({0b0000'0111, 0b0000'0000});

    // 3
    fillEncoded({0b0000'0011, 0b0000'0011});

    // Data length = 4 (0b...0100)
    fillEncoded(std::vector<Byte>(7, Byte{}));
    fillEncoded({0b0000'0100});

    // L = 0100 1100
    fillEncoded({0b0000'0100, 0b0000'1100});

    // y = 0111 1001
    fillEncoded({0b0000'0111, 0b0000'1001});

    // n = 0110 1110
    fillEncoded({0b0000'0110, 0b0000'1110});

    // x = 0111 1000
    fillEncoded({0b0000'0111, 0b0000'1000});

    auto decoded = decode();
    ASSERT_TRUE(decoded.has_value());

    const auto [data, filename] = decoded.value();
    EXPECT_EQ(filename, expectedFilename);
    EXPECT_EQ(data, expectedData);
}

}
