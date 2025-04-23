#pragma once

#include "coder/coder_data.hpp"
#include "encoder.hpp"
#include "util/types.hpp"
#include <expected>
#include <span>
#include <string>
#include <vector>

namespace whisp
{

class RgbEncoder : public Encoder
{
  public:
    RgbEncoder(const std::span<Byte>& data, const int bitsPerChannel);
    EncodeResult encode(const CoderData& dataToEncode) override;

  private:
    std::size_t bytesToEncode(const std::vector<Byte>& buffer, const std::string& filename) const;
    std::size_t maxBytesToEncode() const;

    template <typename T>
    void encode(const std::span<T> dataToEncode)
    {
        encode(dataToEncode.size());

        for (const Byte byte : dataToEncode)
        {
            encode(byte);
        }
    }

    void encode(const std::size_t size);
    void encode(const Byte byte);

    std::span<Byte> data;
    int bitsPerChannel;
    std::span<Byte>::iterator iterator;
};

}
