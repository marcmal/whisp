###################################
# Setup CMake                     #
###################################
cmake_minimum_required(VERSION 3.27)

###################################
# Setup project                   #
###################################

project(fictional-telegram VERSION 1.0 DESCRIPTION "TODO" LANGUAGES CXX)
add_subdirectory(src)


###################################
# Setup tests                     #
###################################

include(CTest)
enable_testing()
add_subdirectory(test)

###################################
# Setup tools                     #
###################################
include(cmake/clang-format.cmake)
