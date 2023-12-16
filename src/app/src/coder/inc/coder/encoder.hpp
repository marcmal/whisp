#pragma once

#include "util/types.hpp"
#include <span>
#include <string>
#include <vector>

namespace fict_tele
{

class Encoder
{
  public:
    Encoder(const std::span<Byte>& data, const int bitsPerChannel);
    void encode(const std::vector<Byte>& buffer, const std::string& filename);

  private:
    std::size_t bytesToEncode(const std::vector<Byte>& buffer, const std::string& filename) const;
    std::size_t maxBytesToEncode() const;

    template <typename T> void encode(const std::span<T> data)
    {
        encode(data.size());

        for (const Byte c : data)
        {
            encode(c);
        }
    }

    void encode(const std::size_t size);
    void encode(const Byte byte);

    const std::span<Byte> data;
    const int bitsPerChannel;
    std::span<Byte>::iterator iterator;
};

}
