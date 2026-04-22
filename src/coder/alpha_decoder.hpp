#pragma once

#include "decoder.hpp"
#include "util/types.hpp"
#include <ranges>
#include <span>
#include <vector>

namespace whisp
{
class AlphaDecoder : public Decoder
{
  public:
    AlphaDecoder(const std::span<Byte>&);
    DecodeResult decode() override;

  private:
    std::size_t decodeLength();
    std::vector<Byte> decodeData(std::size_t length);
    Byte decodeByte();

    using StridedView = decltype(std::declval<std::span<Byte>>() | std::views::drop(3) | std::views::stride(4));
    StridedView view;
    std::ranges::iterator_t<StridedView> it;
};

}
