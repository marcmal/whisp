find_package(GTest REQUIRED)

function(enable_coverage name)
  target_compile_options(${name} PUBLIC --coverage)
  target_link_libraries(${name} PUBLIC gcov)
endfunction()

if (COVERAGE)
  enable_coverage(parser-lib)
  enable_coverage(coder-lib)
  enable_coverage(file-lib)
endif()

add_executable(parser_test parser/cli_parser_test.cpp)
target_link_libraries(parser_test PRIVATE parser-lib GTest::GTest gtest_main)
add_test(NAME ParserTest COMMAND parser_test)


add_executable(coder_test coder/encoder_test.cpp 
                          coder/decoder_test.cpp
                          coder/encoder_decoder_test.cpp)
target_link_libraries(coder_test PRIVATE coder-lib GTest::GTest gtest_main)
add_test(NAME CoderTest COMMAND coder_test)


add_executable(file_test file/reader_writer_test.cpp)
target_link_libraries(file_test PRIVATE file-lib GTest::GTest gtest_main)
add_test(NAME FileTest COMMAND file_test)


add_custom_target(tests DEPENDS parser_test coder_test file_test)
