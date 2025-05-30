# sets default target properties
function(set_common_target_compile_properties target)
    target_compile_features(${target} PRIVATE cxx_std_23)
    target_compile_options(${target} PRIVATE
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:Linux>>:-stdlib=libc++>)

    target_link_options(${target} PRIVATE
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:Linux>>:-stdlib=libc++ -lc++abi>)
endfunction()

function(enable_coverage target scope)
  target_compile_options(${target} ${scope} --coverage)
  target_link_libraries(${target} ${scope} gcov)
  message(STATUS "🟢 coverage enabled on ${target} target.")
endfunction()

function(setup_sanitizers target)
    if (SANITIZERS_ENABLE)
        set(SANITIZERS -fsanitize=address -fsanitize=leak -fsanitize=undefined)

        target_compile_options(${target} PUBLIC ${SANITIZERS})
        target_link_options(${target} PUBLIC ${SANITIZERS})

        message(STATUS "🟢 configured sanitizers for ${target} target.")
    endif()
endfunction()

function(setup_clang_tidy target)
  if (CLANG_TIDY_ENABLE)
        find_program(CLANG_TIDY_EXE NAMES clang-tidy)

        set(CLANG_TIDY_OPTS "--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
        set(CLANG_TIDY_COMMAND ${CLANG_TIDY_EXE} ${CLANG_TIDY_OPTS})

        set_target_properties(${target} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
        message(STATUS "🟢 clang-tidy added to ${target} target.")
    endif()
endfunction()
