cmake_minimum_required(VERSION 3.27)
project(fictional-telegram VERSION 1.0 LANGUAGES CXX)

include(common)
set(target fictional-telegram)

add_subdirectory(app)

add_executable(${target} main.cpp)
set_common_target_compile_properties(${target})
target_link_libraries(${target} PRIVATE app)

setup_clang_tidy(${target})
setup_sanitizers(${target})