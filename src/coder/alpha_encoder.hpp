#pragma once

#include "encoder.hpp"
#include "util/types.hpp"
#include <ranges>
#include <span>

namespace whisp
{

class AlphaEncoder : public Encoder
{
  public:
    explicit AlphaEncoder(const std::span<Byte>& data);

  private:
    std::size_t maxBytesToEncode() const override;
    void encode(const Byte byte) override;

    using StridedView = decltype(std::declval<std::span<Byte>>() | std::views::drop(3) | std::views::stride(4));
    StridedView view;
    std::ranges::iterator_t<StridedView> it;
};

}
