#include "app.hpp"
#include <filesystem>
#include <fstream>
#include <gmock/gmock.h>

namespace fict_tele
{

struct AppTestData
{
    std::string image{};
    std::string secret{};

    std::string input{};
    std::string encoded{};
    std::string decoded{};
    int bitsPerChannel{};
};

namespace
{
AppTestData TEST_DATA;
}

class AppIntegrationTest : public testing::Test
{
  public:
    AppIntegrationTest()
    {
        clean();
        std::filesystem::copy_file(TEST_DATA.secret, TEST_DATA.input);
    }

    ~AppIntegrationTest() override
    {
        clean();
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

    void clean()
    {
        std::filesystem::remove(TEST_DATA.input);
        std::filesystem::remove(TEST_DATA.encoded);
        std::filesystem::remove(TEST_DATA.decoded);
    }

    App app{};
};

TEST_F(AppIntegrationTest, EncodeDecode)
{
    {
        const auto argc = 8;
        const char* argv[] = {"fictional_telegram",
                              "--encode",
                              "--image",
                              TEST_DATA.image.c_str(),
                              "--file",
                              TEST_DATA.input.c_str(),
                              "--bits",
                              std::to_string(TEST_DATA.bitsPerChannel).c_str()};
        app.run(argc, argv);
    }

    std::filesystem::remove(TEST_DATA.input);

    {
        const auto argc = 6;
        const char* argv[] = {"fictional_telegram",      "--decode", "--image",
                              TEST_DATA.encoded.c_str(), "--bits",   std::to_string(TEST_DATA.bitsPerChannel).c_str()};
        app.run(argc, argv);
    }

    EXPECT_TRUE(compareFiles(TEST_DATA.secret, TEST_DATA.decoded));
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

    fict_tele::TEST_DATA = {.image = resourceDir + "/data/image.png",
                            .secret = resourceDir + "/data/secret.png",
                            .input = resourceDir + "/data/temp.png",
                            .encoded = resourceDir + "/data/encoded_image.png",
                            .decoded = resourceDir + "/data/temp.png",
                            .bitsPerChannel = 2};

    return RUN_ALL_TESTS();
}