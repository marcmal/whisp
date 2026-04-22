include(common)

add_library(io reader.cpp
               writer.cpp
               image_reader.cpp)
set_common_target_compile_properties(io)

target_include_directories(io PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(CImg REQUIRED)
find_package(PNG REQUIRED)

target_compile_definitions(io PUBLIC cimg_display=0 cimg_use_png)
target_link_libraries(io PUBLIC util)
target_link_libraries(io PRIVATE CImg::CImg PNG::PNG)

setup_clang_tidy(io)
setup_sanitizers(io)

if (COVERAGE_ENABLE)
  enable_coverage(io PUBLIC)
endif()

if (COVERAGE_SOURCE_ENABLE)
  enable_source_based_coverage(io PUBLIC)
endif()
