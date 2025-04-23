#include "parser/error.hpp"
#include <gtest/gtest.h>

namespace whisp
{

TEST(ErrorTest, FormatEncodeError)
{
    const ParserError err{"Some parsing exception description."};
    const auto result = fmt::format("{}", err);
    EXPECT_EQ(result, "[ParserError: Some parsing exception description.]");
}

}
