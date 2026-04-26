find_package(GTest REQUIRED)
include(common)

add_executable(parser_test parser/arg_parser_test.cpp
                           parser/error_test.cpp
                           parser/config_test.cpp)
set_common_target_compile_properties(parser_test)
setup_sanitizers(parser_test)
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
setup_sanitizers(coder_test)
target_link_libraries(coder_test PRIVATE coder GTest::GTest GTest::gtest_main)
add_test(NAME CoderTest COMMAND coder_test)

add_executable(io_test io/file_reader_writer_test.cpp)
set_common_target_compile_properties(io_test)
setup_sanitizers(io_test)
target_link_libraries(io_test PRIVATE io GTest::GTest GTest::gtest_main)
add_test(NAME IOTest COMMAND io_test)

add_executable(app_test app/app_test.cpp)
set_common_target_compile_properties(app_test)
setup_sanitizers(app_test)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(app_test PRIVATE app GTest::GTest spdlog::spdlog)
add_test(NAME AppTest COMMAND app_test ${CMAKE_CURRENT_SOURCE_DIR}/app)

set(TEST_TARGETS parser_test coder_test io_test app_test)
set(LIB_TARGETS parser coder io app)
add_custom_target(tests DEPENDS ${TEST_TARGETS})

if (COVERAGE_ENABLE)
  add_custom_target(coverage
                    COMMAND ctest -C Debug --test-dir ${CMAKE_BINARY_DIR}
                    COMMAND gcovr --exclude test --exclude ${CMAKE_BINARY_DIR}/vcpkg_installed
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    )
  add_dependencies(coverage tests)
endif()

if (COVERAGE_SOURCE_ENABLE)
  set(COVERAGE_IGNORE_REGEX --ignore-filename-regex=test/|vcpkg_installed/)

  set(COVERAGE_OBJECTS)
  foreach(target ${LIB_TARGETS})
    list(APPEND COVERAGE_OBJECTS -object $<TARGET_FILE:${target}>)
  endforeach()

  file(WRITE ${CMAKE_BINARY_DIR}/merge_profraw.cmake
    "file(GLOB profraw_files \"${CMAKE_BINARY_DIR}/*.profraw\")\n"
    "execute_process(COMMAND llvm-profdata merge -sparse \${profraw_files} -o ${CMAKE_BINARY_DIR}/whisp.profdata)\n"
  )

  add_custom_target(coverage_source
    COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE=${CMAKE_BINARY_DIR}/%p.profraw
            ctest -C Debug --output-on-failure
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/merge_profraw.cmake
    COMMAND llvm-cov report
            ${COVERAGE_OBJECTS}
            -instr-profile=${CMAKE_BINARY_DIR}/whisp.profdata
            ${COVERAGE_IGNORE_REGEX}
    DEPENDS tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    VERBATIM
  )

  add_custom_target(coverage_source_html
    COMMAND llvm-cov show
            ${COVERAGE_OBJECTS}
            -instr-profile=${CMAKE_BINARY_DIR}/whisp.profdata
            ${COVERAGE_IGNORE_REGEX}
            --show-branches=count
            --format=html
            --output-dir=${CMAKE_BINARY_DIR}/coverage_html
    DEPENDS coverage_source
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    VERBATIM
  )
endif()
