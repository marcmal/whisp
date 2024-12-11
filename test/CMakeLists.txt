find_package(GTest REQUIRED)
include(common)

add_executable(parser_test parser/cli_parser_test.cpp)
set_common_target_compile_properties(parser_test)
target_link_libraries(parser_test PRIVATE parser-lib GTest::GTest GTest::gtest_main)
add_test(NAME ParserTest COMMAND parser_test)


add_executable(coder_test coder/encoder_test.cpp 
                          coder/decoder_test.cpp
                          coder/encoder_decoder_test.cpp)
set_common_target_compile_properties(coder_test)
target_link_libraries(coder_test PRIVATE coder-lib GTest::GTest GTest::gtest_main)
add_test(NAME CoderTest COMMAND coder_test)


add_executable(file_test file/reader_writer_test.cpp)
set_common_target_compile_properties(file_test)
target_link_libraries(file_test PRIVATE file-lib GTest::GTest GTest::gtest_main)
add_test(NAME FileTest COMMAND file_test)

add_executable(app_test app/app_test.cpp)
set_common_target_compile_properties(app_test)
target_link_libraries(app_test PRIVATE app GTest::GTest)
add_test(NAME AppTest COMMAND app_test ${CMAKE_CURRENT_SOURCE_DIR}/app)

add_custom_target(tests)
add_dependencies(tests parser_test coder_test file_test app_test)

if (COVERAGE_ENABLE)
  enable_coverage(parser-lib)
  enable_coverage(coder-lib)
  enable_coverage(file-lib)
  enable_coverage(app)
  enable_coverage(fictional-telegram)

  add_custom_target(coverage 
                    COMMAND ctest -C Debug --test-dir ${CMAKE_BINARY_DIR}
                    COMMAND gcovr --exclude test --exclude ${CMAKE_BINARY_DIR}/vcpkg_installed
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    )
  add_dependencies(coverage tests)
endif()