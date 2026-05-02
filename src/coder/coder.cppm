module;

#include <span>

export module whisp.coder;

export import :types;
export import :error;
import whisp.util;
import whisp.types;

namespace whisp::coder
{
using whisp::util::Byte;

export EncodeResult encode(const algorithm::Config& algorithmConfig,
                           std::span<Byte> buffer,
                           const CoderData& coderData);

export DecodeResult decode(const std::span<Byte>& buffer);

}