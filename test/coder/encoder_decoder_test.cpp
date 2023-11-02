#include "coder/inc/decoder.hpp"
#include "coder/inc/encoder.hpp"
#include "gtest/gtest.h"
#include <CImg.h>
#include <gtest/gtest.h>
#include <iterator>

namespace stegan
{

using BitsPerChannel = int;

class EncoderDecoderTestSuite : public testing::TestWithParam<BitsPerChannel>
{
  public:
    EncoderDecoderTestSuite() : image(64, 64, 1, 3, {})
    {
    }

    void testEncodeDecode(const BitsPerChannel bitsPerChannel)
    {
        std::vector<Byte> buffer;
        std::transform(expectedMessage.begin(), expectedMessage.end(), std::back_inserter(buffer),
                       [](const auto c) { return c; });
        Encoder encoder{image, bitsPerChannel};

        encoder.encode(buffer);

        Decoder decoder{image, bitsPerChannel};
        const auto decoded = decoder.decode();

        std::string decodedMessage{decoded.begin(), decoded.end()};
        EXPECT_EQ(decodedMessage, expectedMessage);
    }

  protected:
    cimg_library::CImg<unsigned char> image;

    const std::string expectedMessage =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt "
        "ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation "
        "ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in "
        "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur "
        "sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id "
        "est laborum.";
};

TEST_P(EncoderDecoderTestSuite, EncodeDecode)
{
    testEncodeDecode(GetParam());
}

INSTANTIATE_TEST_CASE_P(EncodeDecodeTests, EncoderDecoderTestSuite, testing::Values(1, 2, 4));

}
