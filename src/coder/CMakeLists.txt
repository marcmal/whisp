add_library(coder-lib src/encoder.cpp src/decoder.cpp)

find_package(CImg REQUIRED)
find_package(PNG REQUIRED)

target_compile_definitions(coder-lib PUBLIC cimg_display=0 cimg_use_png)
target_link_libraries(coder-lib PUBLIC util-lib CImg::CImg PNG::PNG)

target_include_directories(
    coder-lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
    PRIVATE ${CMAKE_SOURCE_DIR}/src)