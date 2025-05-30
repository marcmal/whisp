#pragma once

#include "coder/result.hpp"
#include "constants.hpp"
#include "util/types.hpp"
#include <expected>
#include <span>
#include <string>
#include <vector>

namespace whisp
{

class Encoder
{
  public:
    virtual ~Encoder() = default;

    EncodeResult encode(const CoderData& dataToEncode)
    {
        if (bytesToEncode(dataToEncode.content, dataToEncode.filename) > maxBytesToEncode())
        {
            return std::unexpected(EncodeError{"Too long message to encode."});
        }

        encode(std::span{dataToEncode.filename});
        encode(std::span{dataToEncode.content});
        return {};
    }

  protected:
    virtual std::size_t maxBytesToEncode() const = 0;
    virtual void encode(const Byte byte) = 0;

  private:
    template <typename T>
    void encode(const std::span<T> dataToEncode)
    {
        encode(dataToEncode.size());

        for (const Byte byte : dataToEncode)
        {
            encode(byte);
        }
    }

    void encode(const std::size_t size)
    {
        for (auto i = 1; i <= NUM_BYTES_LENGTH_ENCODED; i++)
        {
            encode(static_cast<Byte>(size >> (NUM_BYTES_LENGTH_ENCODED - i) * BITS_IN_BYTE));
        }
    }

    std::size_t bytesToEncode(const std::vector<Byte>& buffer, const std::string& filename) const
    {
        return NUM_BYTES_LENGTH_ENCODED + filename.size() + NUM_BYTES_LENGTH_ENCODED + buffer.size();
    }
};

}