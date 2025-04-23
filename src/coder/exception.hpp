#pragma once

#include "coder/error.hpp"

namespace whisp
{

struct DecodeException
{
    DecodeError cause;
};

struct EncodeException
{
    EncodeError cause;
};

}
