#pragma once

#include "coder/result.hpp"
#include "util/types.hpp"
#include <span>

namespace whisp
{
struct Header;

EncodeResult encodeHeader(const Header& header, std::span<Byte> buffer);

}