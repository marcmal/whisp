#include "app.hpp"
#include <filesystem>
#include <fstream>
#include <gmock/gmock.h>

namespace fict_tele
{

std::string image{};
std::string secret{};

std::string input{};
std::string encoded{};
std::string decoded{};
const auto bitsPerChannel = "2";

class AppIntegrationTest : public testing::Test
{
  public:
    AppIntegrationTest()
    {
        std::filesystem::copy_file(secret, input);
    }

    ~AppIntegrationTest() override
    {
        std::filesystem::remove(input);
        std::filesystem::remove(encoded);
        std::filesystem::remove(decoded);
    }

  protected:
    bool compareFiles(const std::string& p1, const std::string& p2)
    {
        std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
        std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);

        if (f1.fail() || f2.fail())
        {
            return false;
        }

        if (f1.tellg() != f2.tellg())
        {
            return false;
        }

        f1.seekg(0, std::ifstream::beg);
        f2.seekg(0, std::ifstream::beg);

        return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()), std::istreambuf_iterator<char>(),
                          std::istreambuf_iterator<char>(f2.rdbuf()));
    }

    App app{};
};

TEST_F(AppIntegrationTest, EncodeDecode)
{
    {
        const auto argc = 8;
        const char* argv[] = {"fictional_telegram", "--encode", "--image",     image.c_str(), "--file",
                              input.c_str(),        "--bits",   bitsPerChannel};
        app.run(argc, argv);
    }

    std::filesystem::remove(input);

    {
        const auto argc = 6;
        const char* argv[] = {"fictional_telegram", "--decode", "--image", encoded.c_str(), "--bits", bitsPerChannel};
        app.run(argc, argv);
    }

    EXPECT_TRUE(compareFiles(secret, decoded));
}

}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    if (argc != 2)
    {
        std::cerr << "Expected one extra argument point test app data" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string resourceDir = argv[1];
    fict_tele::image = resourceDir + "/data/image.png";
    fict_tele::secret = resourceDir + "/data/secret.png";

    fict_tele::input = resourceDir + "/data/temp.png";
    fict_tele::encoded = resourceDir + "/data/encoded_image.png";
    fict_tele::decoded = resourceDir + "/data/temp.png";

    return RUN_ALL_TESTS();
}