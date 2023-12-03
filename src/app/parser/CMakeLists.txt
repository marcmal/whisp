add_library(parser-lib src/cli_parser.cpp)

find_package(cxxopts REQUIRED)
target_link_libraries(parser-lib PUBLIC cxxopts::cxxopts)

target_include_directories(
    parser-lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

if (LINT)
    include(${CMAKE_SOURCE_DIR}/cmake/clang-tidy.cmake)
    setup_clang_tidy(parser-lib ${SOURCES})
endif()