#pragma once

#include "coder/error.hpp"
#include "util/types.hpp"
#include <expected>
#include <span>

namespace whisp
{
struct Header;

std::expected<Header, DecodeError> decodeHeader(const std::span<Byte>& data);

}