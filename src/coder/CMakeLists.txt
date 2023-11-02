set(SOURCE_FILES src/encoder.cpp src/decoder.cpp)

add_library(coder-lib ${SOURCE_FILES})

find_package(cimg REQUIRED)
find_package(PNG REQUIRED)

target_compile_definitions(coder-lib PUBLIC cimg_display=0 cimg_use_png)
target_compile_features(coder-lib PUBLIC cxx_std_20)
target_link_libraries(coder-lib PUBLIC cimg::cimg PNG::PNG)

target_include_directories(
    coder-lib
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src)

