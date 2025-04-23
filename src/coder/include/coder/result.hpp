#pragma once

#include "coder/coder_data.hpp"
#include "coder/error.hpp"
#include <expected>

namespace whisp
{
using EncodeResult = std::expected<void, EncodeError>;
using DecodeResult = std::expected<CoderData, DecodeError>;
}