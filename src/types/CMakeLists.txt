include(common)

add_library(types)
set_common_target_compile_properties(types)

target_sources(types PUBLIC
    FILE_SET CXX_MODULES FILES
    algorithm.cppm
)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(types PUBLIC spdlog::spdlog)

setup_clang_tidy(types)
setup_sanitizers(types)

if(COVERAGE_ENABLE)
    enable_coverage(types PUBLIC)
endif()

if(COVERAGE_SOURCE_ENABLE)
    enable_source_based_coverage(types PUBLIC)
endif()
