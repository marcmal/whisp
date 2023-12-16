add_subdirectory(src/util)
add_subdirectory(src/coder)
add_subdirectory(src/parser)
add_subdirectory(src/file)

add_library(app src/app.cpp)
target_link_libraries(app PRIVATE coder-lib parser-lib file-lib)

find_package(CImg REQUIRED)
find_package(PNG REQUIRED)
target_compile_definitions(app PUBLIC cimg_display=0 cimg_use_png)
target_link_libraries(app PRIVATE CImg::CImg PNG::PNG)

target_include_directories(
    app
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

if (ANALYSIS_ENABLE)
    include(${CMAKE_SOURCE_DIR}/cmake/analysis.cmake)
    setup_clang_tidy(app)
endif()

if (SANITIZERS_ENABLE)
    include(${CMAKE_SOURCE_DIR}/cmake/sanitizers.cmake)
    setup_sanitizers(app) 
endif()