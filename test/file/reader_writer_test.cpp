#include "file/reader.hpp"
#include "file/writer.hpp"
#include <filesystem>
#include <gtest/gtest.h>

namespace fict_tele
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
    FileReader reader{filename};
    FileWriter writer{filename};
};

TEST_F(ReaderWriterTest, WriteReadFile)
{
    std::vector<Byte> data;
    std::copy(content.begin(), content.end(), std::back_inserter(data));

    writer.write(data);
    EXPECT_EQ(data, reader.read());
}

}
