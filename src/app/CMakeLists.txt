include(common)
set(target app)

add_subdirectory(src/util)
add_subdirectory(src/coder)
add_subdirectory(src/parser)
add_subdirectory(src/file)

add_library(${target} src/app.cpp)
set_common_target_compile_properties(${target})
set_common_target_include_directories(${target} PUBLIC)
target_link_libraries(${target} PRIVATE coder-lib parser-lib file-lib)

find_package(CImg REQUIRED)
find_package(PNG REQUIRED)

target_compile_definitions(${target} PUBLIC cimg_display=0 cimg_use_png)
target_link_libraries(${target} PRIVATE CImg::CImg PNG::PNG)
target_compile_options(${target} PRIVATE -Wno-error=format-truncation)

setup_clang_tidy(${target})
setup_sanitizers(${target}) 