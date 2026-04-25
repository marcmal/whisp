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

  private:
    std::size_t decodeLength() override;
    std::vector<Byte> decodeData(std::size_t length) override;
    Byte decodeByte() override;

    using StridedView = decltype(std::declval<std::span<Byte>>() | std::views::drop(3) | std::views::stride(4));
    StridedView view;
    std::ranges::iterator_t<StridedView> it;
};

}
