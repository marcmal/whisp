add_library(coder-lib src/encoder.cpp src/decoder.cpp)

target_link_libraries(coder-lib PUBLIC util-lib)

target_include_directories(
    coder-lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

if (LINT)
    include(${CMAKE_SOURCE_DIR}/cmake/clang-tidy.cmake)
    setup_clang_tidy(coder-lib ${SOURCES})
endif()