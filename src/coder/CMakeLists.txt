include(common)

add_library(coder)
set_common_target_compile_properties(coder)

target_sources(coder PUBLIC
    FILE_SET CXX_MODULES FILES
    coder.cppm base.cppm types.cppm rgb.cppm alpha.cppm header.cppm util.cppm
)

find_package(spdlog CONFIG REQUIRED)

target_link_libraries(coder PUBLIC parser)
target_link_libraries(coder PRIVATE spdlog::spdlog)

setup_clang_tidy(coder)
setup_sanitizers(coder)

if(COVERAGE_ENABLE)
    enable_coverage(coder PUBLIC)
endif()

if(COVERAGE_SOURCE_ENABLE)
    enable_source_based_coverage(coder PUBLIC)
endif()
