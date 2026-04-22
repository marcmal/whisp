include(common)

add_library(coder
    encode.cpp
    decode.cpp
    header_encoder.cpp
    header_decoder.cpp
    alpha_encoder.cpp
    alpha_decoder.cpp
    rgb_encoder.cpp
    rgb_decoder.cpp)
set_common_target_compile_properties(coder)

find_package(spdlog CONFIG REQUIRED)

target_include_directories(coder PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(coder PUBLIC util parser)
target_link_libraries(coder PRIVATE spdlog::spdlog)

setup_clang_tidy(coder)
setup_sanitizers(coder)

if (COVERAGE_ENABLE)
  enable_coverage(coder PUBLIC)
endif()

if (COVERAGE_SOURCE_ENABLE)
  enable_source_based_coverage(coder PUBLIC)
endif()
