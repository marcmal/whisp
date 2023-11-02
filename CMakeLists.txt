###################################
# Setup CMake                     #
###################################
cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###################################
# Setup project                   #
###################################

project(fictional-telegram)
add_subdirectory(src)


###################################
# Setup tests                     #
###################################

enable_testing()
add_subdirectory(test)

###################################
# Setup tools                     #
###################################
include(cmake/clang-format.cmake)
