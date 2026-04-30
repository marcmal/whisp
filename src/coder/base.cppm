module;

#include <cstdint>
#include <expected>
#include <span>
#include <string>
#include <vector>

export module whisp.coder:base;
import :types;
import whisp.util;

namespace whisp::coder
{
class Encoder
{
  public:
    virtual ~Encoder() = default;

    EncodeResult encode(const CoderData& dataToEncode)
    {
        if (bytesToEncode(dataToEncode.content, dataToEncode.secretFileName) > maxBytesToEncode())
        {
            return std::unexpected(EncodeError{"Too long message to encode."});
        }

        encode(std::span{dataToEncode.secretFileName});
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

class Decoder
{
  public:
    virtual ~Decoder() = default;

    DecodeResult decode()
    {
        try
        {
            const auto filenameLength = decodeLength();
            const auto filename = decodeData(filenameLength);

            const auto dataLength = decodeLength();
            const auto decodedData = decodeData(dataLength);
            return CoderData{decodedData, std::string{filename.begin(), filename.end()}};
        }
        catch (const DecodeException& e)
        {
            return std::unexpected(e.cause);
        }
    }

  protected:
    virtual std::size_t decodeLength() = 0;
    virtual std::vector<Byte> decodeData(std::size_t length) = 0;
    virtual Byte decodeByte() = 0;
};

}