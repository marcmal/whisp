add_library(file-lib src/reader.cpp src/writer.cpp)
target_link_libraries(file-lib PUBLIC util-lib)

target_include_directories(
    file-lib
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src)

