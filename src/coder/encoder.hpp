#pragma once

#include "coder/coder_data.hpp"
#include "coder/result.hpp"
#include <expected>

namespace whisp
{

class Encoder
{
  public:
    virtual EncodeResult encode(const CoderData& dataToEncode) = 0;
    virtual ~Encoder() = default;
};

}