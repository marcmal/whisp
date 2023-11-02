set(SOURCE_FILES src/cli_parser.cpp)

add_library(parser-lib ${SOURCE_FILES})

find_package(cxxopts REQUIRED)
target_link_libraries(parser-lib PUBLIC cxxopts::cxxopts)

target_include_directories(
    parser-lib
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src)

