module;

#include <span>

export module whisp.coder;

export import :types;
import whisp.util;
import whisp.parser;

namespace whisp::coder
{
using whisp::util::Byte;

export EncodeResult encode(const parser::AlgorithmConfig& algorithmConfig,
                           std::span<Byte> buffer,
                           const CoderData& coderData);

export DecodeResult decode(const std::span<Byte>& buffer);

}