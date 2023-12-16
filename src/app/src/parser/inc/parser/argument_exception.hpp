#pragma once

#include <exception>
#include <stdexcept>

namespace fict_tele
{

struct ArgumentException : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

}
