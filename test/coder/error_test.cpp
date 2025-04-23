#include "coder/error.hpp"
#include <gtest/gtest.h>

namespace whisp
{

TEST(ErrorTest, FormatEncodeError)
{
    const EncodeError err{"Some encoding exception description."};
    const auto result = fmt::format("{}", err);
    EXPECT_EQ(result, "[EncodeError: Some encoding exception description.]");
}

TEST(ErrorTest, FormatDecodeError)
{
    const DecodeError err{"Some decoding exception description."};
    const auto result = fmt::format("{}", err);
    EXPECT_EQ(result, "[DecodeError: Some decoding exception description.]");
}

}
