#pragma once

#include "coder/result.hpp"
#include <expected>

namespace whisp
{

class Decoder
{
  public:
    virtual DecodeResult decode() = 0;
    virtual ~Decoder() = default;
};

}