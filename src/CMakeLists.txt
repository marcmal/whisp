add_subdirectory(app)

add_executable(fictional-telegram main.cpp)
target_link_libraries(fictional-telegram PRIVATE app)

if (ANALYSIS_ENABLE)
    include(${CMAKE_SOURCE_DIR}/cmake/analysis.cmake)
    setup_clang_tidy(fictional-telegram)
endif()
