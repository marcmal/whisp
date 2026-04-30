#include <fmt/format.h>
#include <gtest/gtest.h>

import whisp.coder;

namespace whisp
{

TEST(ErrorTest, FormatEncodeError)
{
    const coder::EncodeError err{"Some encoding exception description."};
    const auto result = fmt::format("{}", err);
    EXPECT_EQ(result, "[EncodeError: Some encoding exception description.]");
}

TEST(ErrorTest, FormatDecodeError)
{
    const coder::DecodeError err{"Some decoding exception description."};
    const auto result = fmt::format("{}", err);
    EXPECT_EQ(result, "[DecodeError: Some decoding exception description.]");
}

}
