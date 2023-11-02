#pragma once

#include <exception>
#include <stdexcept>

namespace stegan
{
struct ArgumentException : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

}
