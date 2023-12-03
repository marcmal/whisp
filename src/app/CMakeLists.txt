add_subdirectory(coder)
add_subdirectory(parser)
add_subdirectory(file)
add_subdirectory(util)

add_library(app src/app.cpp)
target_link_libraries(app PRIVATE coder-lib parser-lib file-lib)

find_package(CImg REQUIRED)
find_package(PNG REQUIRED)
target_compile_definitions(app PUBLIC cimg_display=0 cimg_use_png)
target_link_libraries(app PRIVATE CImg::CImg PNG::PNG)

target_include_directories(
    app
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

if (LINT)
    include(${CMAKE_SOURCE_DIR}/cmake/clang-tidy.cmake)
    setup_clang_tidy(app ${SOURCES})
endif()