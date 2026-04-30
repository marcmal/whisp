include(common)

add_subdirectory(coder)
add_subdirectory(parser)
add_subdirectory(io)
add_subdirectory(util)

add_library(app)
set_common_target_compile_properties(app)

target_sources(app PUBLIC
    FILE_SET CXX_MODULES FILES
    app.cppm
)

target_link_libraries(app PRIVATE coder parser io)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(app PRIVATE spdlog::spdlog)

setup_clang_tidy(app)
setup_sanitizers(app)

if(COVERAGE_ENABLE)
    enable_coverage(app PUBLIC)
endif()

if(COVERAGE_SOURCE_ENABLE)
    enable_source_based_coverage(app PUBLIC)
endif()

add_executable(whisp main.cpp)
set_common_target_compile_properties(whisp)
target_link_libraries(whisp PRIVATE app)

setup_clang_tidy(whisp)
setup_sanitizers(whisp)

if(COVERAGE_ENABLE)
    enable_coverage(whisp PUBLIC)
endif()

if(COVERAGE_SOURCE_ENABLE)
    enable_source_based_coverage(whisp PUBLIC)
endif()
