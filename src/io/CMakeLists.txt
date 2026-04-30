include(common)

add_library(io)
set_common_target_compile_properties(io)

target_sources(io PUBLIC
    FILE_SET CXX_MODULES FILES
    io.cppm reader.cppm writer.cppm image_reader.cppm
)

find_package(CImg REQUIRED)
find_package(PNG REQUIRED)

target_compile_definitions(io PUBLIC cimg_display=0 cimg_use_png)
target_link_libraries(io PUBLIC util)
target_link_libraries(io PRIVATE CImg::CImg PNG::PNG)

setup_clang_tidy(io)
setup_sanitizers(io)

if(COVERAGE_ENABLE)
    enable_coverage(io PUBLIC)
endif()

if(COVERAGE_SOURCE_ENABLE)
    enable_source_based_coverage(io PUBLIC)
endif()
