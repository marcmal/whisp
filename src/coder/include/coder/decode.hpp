#pragma once

#include "coder/result.hpp"
#include "util/types.hpp"
#include <span>

namespace whisp
{
[[nodiscard]] DecodeResult decode(const std::span<Byte>& data);
}