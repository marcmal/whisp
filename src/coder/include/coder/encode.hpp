#pragma once

#include "coder/result.hpp"
#include "parser/config.hpp"
#include "util/types.hpp"
#include <span>

namespace whisp
{

struct CoderData;

[[nodiscard]] EncodeResult encode(const AlgorithmConfig& algorithmConfig,
                                  std::span<Byte> buffer,
                                  const CoderData& coderData);

}
