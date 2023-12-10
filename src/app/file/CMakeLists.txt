add_library(file-lib src/reader.cpp src/writer.cpp)

target_link_libraries(file-lib PUBLIC util-lib)

target_include_directories(
    file-lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

if (LINT)
    include(${CMAKE_SOURCE_DIR}/cmake/clang-tidy.cmake)
    setup_clang_tidy(file-lib)
endif()

if (SANITIZERS_ENABLE)
    include(${CMAKE_SOURCE_DIR}/cmake/sanitizers.cmake)
    setup_sanitizers(file-lib) 
endif()