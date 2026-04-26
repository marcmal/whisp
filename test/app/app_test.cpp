#include "app.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

namespace whisp
{

namespace
{

struct ImageInputOutput
{
    std::string image;
    std::string encoded;
};

struct AppTestData
{
    std::string image{};
    std::string imageEncoded{};

    std::string smallImage{};

    std::string rgbImage{};
    std::string rgbImageEncoded{};

    std::string secret{};

    std::string input{};

    std::string decoded{};
};

AppTestData TEST_DATA;

template <typename T, std::size_t N>
constexpr std::size_t arraySize(const T (&)[N]) noexcept
{
    return N;
}

}

class AppIntegrationTest : public testing::Test
{
  public:
    AppIntegrationTest()
    {
        std::filesystem::copy_file(TEST_DATA.secret, TEST_DATA.input, std::filesystem::copy_options::update_existing);
    }

    ~AppIntegrationTest() override
    {
        std::filesystem::remove(TEST_DATA.input);
        std::filesystem::remove(TEST_DATA.imageEncoded);
        std::filesystem::remove(TEST_DATA.decoded);
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

        return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                          std::istreambuf_iterator<char>(),
                          std::istreambuf_iterator<char>(f2.rdbuf()));
    }

    App app{};
};

TEST_F(AppIntegrationTest, EncodeDecodeRgbMode)
{
    {
        const char* argv[] = {"whisp",
                              "encode",
                              "--image-file",
                              TEST_DATA.image.c_str(),
                              "--secret-file",
                              TEST_DATA.input.c_str(),
                              "rgb",
                              "--bits",
                              "2"};
        app.run(arraySize(argv), argv);
    }

    std::filesystem::remove(TEST_DATA.input);

    {
        const char* argv[] = {"whisp", "decode", "--image-file", TEST_DATA.imageEncoded.c_str()};
        app.run(arraySize(argv), argv);
    }

    EXPECT_TRUE(compareFiles(TEST_DATA.secret, TEST_DATA.decoded));
}

TEST_F(AppIntegrationTest, EncodeDecodeAlphaMode)
{
    {
        const char* argv[] = {"whisp",
                              "encode",
                              "--image-file",
                              TEST_DATA.image.c_str(),
                              "--secret-file",
                              TEST_DATA.input.c_str(),
                              "alpha"};
        app.run(arraySize(argv), argv);
    }

    std::filesystem::remove(TEST_DATA.input);

    {
        const char* argv[] = {"whisp", "decode", "--image-file", TEST_DATA.imageEncoded.c_str()};
        app.run(arraySize(argv), argv);
    }

    EXPECT_TRUE(compareFiles(TEST_DATA.secret, TEST_DATA.decoded));
}

TEST_F(AppIntegrationTest, EncodeAlphaInRgbImage)
{
    {
        const char* argv[] = {"whisp",
                              "encode",
                              "--image-file",
                              TEST_DATA.rgbImage.c_str(),
                              "--secret-file",
                              TEST_DATA.input.c_str(),
                              "alpha"};
        app.run(arraySize(argv), argv);
    }

    std::filesystem::remove(TEST_DATA.input);

    {
        const char* argv[] = {"whisp", "decode", "--image-file", TEST_DATA.rgbImageEncoded.c_str()};
        app.run(arraySize(argv), argv);
    }

    EXPECT_TRUE(compareFiles(TEST_DATA.secret, TEST_DATA.decoded));
}

TEST_F(AppIntegrationTest, Help)
{
    const char* argv[] = {"whisp", "--help"};
    EXPECT_EQ(app.run(arraySize(argv), argv), EXIT_SUCCESS);
}

TEST_F(AppIntegrationTest, SetDebugLevelWhenVerboseMode)
{
    {
        const char* argv[] = {"whisp",
                              "encode",
                              "--image-file",
                              TEST_DATA.rgbImage.c_str(),
                              "--secret-file",
                              TEST_DATA.input.c_str(),
                              "alpha",
                              "--verbose"};
        EXPECT_EQ(app.run(arraySize(argv), argv), EXIT_SUCCESS);
    }
    EXPECT_EQ(spdlog::get_level(), spdlog::level::debug);
}

TEST_F(AppIntegrationTest, WrongArgumentsReturnsFailure)
{
    const char* argv[] = {"whisp", "encode", "decode"};
    EXPECT_EQ(app.run(arraySize(argv), argv), EXIT_FAILURE);
}

TEST_F(AppIntegrationTest, EncodeFailureReturnsFailure)
{
    const char* argv[] = {"whisp",
                          "encode",
                          "--image-file",
                          TEST_DATA.smallImage.c_str(),
                          "--secret-file",
                          TEST_DATA.input.c_str(),
                          "rgb",
                          "--bits",
                          "1"};
    EXPECT_EQ(app.run(arraySize(argv), argv), EXIT_FAILURE);
}

TEST_F(AppIntegrationTest, DecodeFailureReturnsFailure)
{
    const char* argv[] = {"whisp", "decode", "--image-file", TEST_DATA.smallImage.c_str()};
    EXPECT_EQ(app.run(arraySize(argv), argv), EXIT_FAILURE);
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

    whisp::TEST_DATA = {.image = resourceDir + "/data/image.png",
                        .imageEncoded = resourceDir + "/data/encoded_image.png",
                        .smallImage = resourceDir + "/data/small_image.png",
                        .rgbImage = resourceDir + "/data/rgb_image.bmp",
                        .rgbImageEncoded = resourceDir + "/data/encoded_rgb_image.bmp",
                        .secret = resourceDir + "/data/secret.png",
                        .input = resourceDir + "/data/temp.png",
                        .decoded = resourceDir + "/data/temp.png"};

    return RUN_ALL_TESTS();
}
