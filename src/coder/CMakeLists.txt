include(common)

add_library(coder)
set_common_target_compile_properties(coder)

target_sources(coder
    PRIVATE coder.cpp

    PUBLIC FILE_SET CXX_MODULES FILES
    coder.cppm
    types.cppm

    # Currently, CMake requires all module implementation partition units to be listed in CXX_MODULES FILES
    # https://gitlab.kitware.com/cmake/cmake/-/work_items/27048
    alpha.cpp
    base.cpp
    header.cpp
    rgb.cpp
    util.cpp
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
