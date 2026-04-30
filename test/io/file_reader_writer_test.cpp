#include <filesystem>
#include <gtest/gtest.h>

import whisp.io;
import whisp.util;

namespace whisp
{

class ReaderWriterTest : public testing::Test
{
  public:
    ReaderWriterTest() = default;

    ~ReaderWriterTest() override
    {
        std::filesystem::remove(filename);
    }

  protected:
    const std::string content{"Example file content"};
    const std::filesystem::path filename{"temp.txt"};
    io::FileReader reader{filename};
    io::FileWriter writer{filename};
};

TEST_F(ReaderWriterTest, WriteReadFile)
{
    std::vector<util::Byte> data;
    std::copy(content.begin(), content.end(), std::back_inserter(data));

    writer.write(data);
    EXPECT_EQ(data, reader.read());
}

}
