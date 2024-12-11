include(common)
set(target parser-lib)

add_library(${target} src/cli_parser.cpp)
set_common_target_compile_properties(${target})
set_common_target_include_directories(${target} PUBLIC)

find_package(cxxopts REQUIRED)
target_link_libraries(${target} PUBLIC cxxopts::cxxopts)

setup_clang_tidy(${target})
setup_sanitizers(${target}) 