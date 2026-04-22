#pragma once

#include "util/types.hpp"
#include <span>

namespace whisp
{
struct Header;

void encodeHeader(const Header& header, std::span<Byte> buffer);

}