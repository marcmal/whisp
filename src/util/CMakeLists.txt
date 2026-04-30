include(common)

add_library(util)
set_common_target_compile_properties(util)

target_sources(util PUBLIC
    FILE_SET CXX_MODULES FILES
    util.cppm
)
