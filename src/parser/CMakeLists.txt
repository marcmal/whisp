include(common)

add_library(parser arg_parser.cpp)
set_common_target_compile_properties(parser)

find_package(CLI11 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_include_directories(parser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(parser PUBLIC CLI11::CLI11 util spdlog::spdlog)

setup_clang_tidy(parser)
setup_sanitizers(parser) 
