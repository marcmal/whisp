###################################
# Setup CMake                     #
###################################
cmake_minimum_required(VERSION 3.30)

###################################
# Setup project                   #
###################################
project(whisp-dev VERSION 1.0 DESCRIPTION "Steganography CLI tool" LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
add_subdirectory(src)

###################################
# Setup tests                     #
###################################
enable_testing()
add_subdirectory(test)

###################################
# Setup tools                     #
###################################
include(format)
