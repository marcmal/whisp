add_subdirectory(coder)
add_subdirectory(parser)
add_subdirectory(file)
add_subdirectory(util)

add_executable(fictional-telegram main.cpp)
target_link_libraries(fictional-telegram PRIVATE util-lib coder-lib parser-lib file-lib)