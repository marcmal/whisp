set(SOURCE_FILES src/reader.cpp src/writer.cpp)

add_library(file-lib ${SOURCE_FILES})

target_include_directories(
    file-lib
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src)

