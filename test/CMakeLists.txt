find_package(GTest REQUIRED)
include(common)

add_executable(parser_test parser/arg_parser_test.cpp
                           parser/error_test.cpp
                           parser/config_test.cpp)
set_common_target_compile_properties(parser_test)
target_link_libraries(parser_test PRIVATE parser GTest::GTest GTest::gtest_main)
add_test(NAME ParserTest COMMAND parser_test)


add_executable(coder_test coder/rgb_encoder_test.cpp 
                          coder/rgb_decoder_test.cpp
                          coder/rgb_encoder_decoder_test.cpp
                          coder/alpha_encoder_test.cpp
                          coder/alpha_decoder_test.cpp
                          coder/alpha_encoder_decoder_test.cpp
                          coder/header_decoder_test.cpp
                          coder/error_test.cpp)
set_common_target_compile_properties(coder_test)
target_link_libraries(coder_test PRIVATE coder GTest::GTest GTest::gtest_main)
add_test(NAME CoderTest COMMAND coder_test)


add_executable(io_test io/file_reader_writer_test.cpp)
set_common_target_compile_properties(io_test)
target_link_libraries(io_test PRIVATE io GTest::GTest GTest::gtest_main)
add_test(NAME IOTest COMMAND io_test)

add_executable(app_test app/app_test.cpp)
set_common_target_compile_properties(app_test)
target_link_libraries(app_test PRIVATE app GTest::GTest)
add_test(NAME AppTest COMMAND app_test ${CMAKE_CURRENT_SOURCE_DIR}/app)

add_custom_target(whisp_tests DEPENDS parser_test coder_test io_test app_test)
add_custom_target(whisp_tests_run
    COMMAND parser_test
    COMMAND coder_test
    COMMAND io_test
    COMMAND app_test ${CMAKE_CURRENT_SOURCE_DIR}/app
    DEPENDS whisp_tests
    COMMENT "Running all tests..."
)


if (COVERAGE_ENABLE)
  enable_coverage(parser)
  enable_coverage(coder)
  enable_coverage(io)
  enable_coverage(app)
  enable_coverage(whisp)

  add_custom_target(coverage 
                    COMMAND ctest -C Debug --test-dir ${CMAKE_BINARY_DIR}
                    COMMAND gcovr --exclude test --exclude ${CMAKE_BINARY_DIR}/vcpkg_installed
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    )
  add_dependencies(coverage whisp_tests_run)
endif()