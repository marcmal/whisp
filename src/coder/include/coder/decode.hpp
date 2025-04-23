#pragma once

#include "coder/result.hpp"
#include "util/types.hpp"
#include <span>

namespace whisp
{
DecodeResult decode(const std::span<Byte>& data);
}