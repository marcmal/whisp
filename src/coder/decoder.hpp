#pragma once

#include "coder/result.hpp"
#include "exception.hpp"

namespace whisp
{

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
