#include <fmt/format.h>
#include <gtest/gtest.h>

import whisp.parser;

namespace whisp
{

TEST(ErrorTest, FormatEncodeError)
{
    const parser::ParserError err{"Some parsing exception description."};
    const auto result = fmt::format("{}", err);
    EXPECT_EQ(result, "[ParserError: Some parsing exception description.]");
}

}
