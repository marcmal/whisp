include(common)
set(target coder-lib)

add_library(${target} src/encoder.cpp src/decoder.cpp)
set_common_target_compile_properties(${target})
set_common_target_include_directories(${target} PUBLIC)

target_link_libraries(${target} PUBLIC util-lib)

setup_clang_tidy(${target})
setup_sanitizers(${target}) 