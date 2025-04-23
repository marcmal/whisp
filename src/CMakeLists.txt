cmake_minimum_required(VERSION 3.27)
project(whisp VERSION 1.0 LANGUAGES CXX)

include(common)

add_subdirectory(coder)
add_subdirectory(parser)
add_subdirectory(io)
add_subdirectory(util)

add_library(app app.cpp)
set_common_target_compile_properties(app)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app PRIVATE coder parser io)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(app PRIVATE spdlog::spdlog)

setup_clang_tidy(app)
setup_sanitizers(app) 

add_executable(whisp main.cpp)
set_common_target_compile_properties(whisp)
target_link_libraries(whisp PRIVATE app)

setup_clang_tidy(whisp)
setup_sanitizers(whisp)