#pragma once

#include "coder/result.hpp"
#include "parser/config.hpp"
#include "util/types.hpp"
#include <expected>
#include <span>

namespace whisp
{

struct CoderData;

EncodeResult encode(const AlgorithmConfig& algorithmConfig, std::span<Byte> buffer, const CoderData& coderData);

}
