add_subdirectory(coder)
add_subdirectory(parser)
add_subdirectory(file)

set(SOURCE_FILES main.cpp)
add_executable(fictional-telegram ${SOURCE_FILES})
target_link_libraries(fictional-telegram PRIVATE coder-lib parser-lib file-lib)
