
#include <gtest/gtest.h>

import whisp.util;
import whisp.coder;
import whisp.parser;

namespace whisp
{
using util::Byte;
using BitsPerChannel = int;

class RgbEncoderDecoderTestSuite : public testing::TestWithParam<BitsPerChannel>
{
  public:
    RgbEncoderDecoderTestSuite() : data(64 * 64 * 3)
    {
    }

    void testEncodeDecode(const BitsPerChannel bitsPerChannel)
    {
        std::vector<Byte> buffer;
        std::transform(
            expectedMessage.begin(), expectedMessage.end(), std::back_inserter(buffer), [](const auto c) { return c; });

        algorithm::RgbConfig config{bitsPerChannel};
        coder::CoderData dataToEncode{buffer, expectedFilename};
        coder::encode(config, std::span{data}, dataToEncode).value();
        const auto [decodedData, decodedFilename] = coder::decode(data).value();

        std::string decodedMessage{decodedData.begin(), decodedData.end()};
        EXPECT_EQ(decodedMessage, expectedMessage);
        EXPECT_EQ(decodedFilename, expectedFilename);
    }

  protected:
    std::vector<Byte> data;

    const std::string expectedFilename = "secret.png";
    const std::string expectedMessage =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt "
        "ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation "
        "ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in "
        "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur "
        "sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id "
        "est laborum.";
};

TEST_P(RgbEncoderDecoderTestSuite, EncodeDecode)
{
    testEncodeDecode(GetParam());
}

INSTANTIATE_TEST_SUITE_P(EncodeDecodeTests, RgbEncoderDecoderTestSuite, testing::Values(1, 2, 4));

}
