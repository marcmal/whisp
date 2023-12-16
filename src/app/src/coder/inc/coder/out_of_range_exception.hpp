#pragma once

#include <exception>
#include <stdexcept>

namespace fict_tele
{

struct OutOfRangeException : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

}
