include(common)

add_library(parser parser.cpp)
set_common_target_compile_properties(parser)

target_sources(parser PUBLIC
    FILE_SET CXX_MODULES FILES
    parser.cppm config.cppm error.cppm
)

find_package(CLI11 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_link_libraries(parser PUBLIC CLI11::CLI11 util types spdlog::spdlog)

setup_clang_tidy(parser)
setup_sanitizers(parser)

if(COVERAGE_ENABLE)
    enable_coverage(parser PUBLIC)
endif()

if(COVERAGE_SOURCE_ENABLE)
    enable_source_based_coverage(parser PUBLIC)
endif()
