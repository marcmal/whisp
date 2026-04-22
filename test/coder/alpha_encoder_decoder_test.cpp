#include "coder/decode.hpp"
#include "coder/encode.hpp"
#include "parser/config.hpp"
#include "util/types.hpp"
#include <gtest/gtest.h>

namespace whisp
{

class AlphaEncoderDecoderTestSuite : public testing::Test
{
  public:
    AlphaEncoderDecoderTestSuite() : data(64 * 64 * 3)
    {
    }

    void testEncodeDecode()
    {
        std::vector<Byte> buffer;
        std::transform(
            expectedMessage.begin(), expectedMessage.end(), std::back_inserter(buffer), [](const auto c) { return c; });

        AlphaAlgorithmConfig config{};
        CoderData dataToEncode{buffer, expectedFilename};
        whisp::encode(config, std::span{data}, dataToEncode).value();
        const auto [decodedData, decodedFilename] = whisp::decode(data).value();

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

TEST_F(AlphaEncoderDecoderTestSuite, EncodeDecode)
{
    testEncodeDecode();
}

}
