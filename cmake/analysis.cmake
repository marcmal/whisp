function(setup_clang_tidy target)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)

    set(CLANG_TIDY_OPTS "--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
    
    set(CLANG_TIDY_COMMAND ${CLANG_TIDY_EXE} ${CLANG_TIDY_OPTS})
    
    set_target_properties(${target} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    message(STATUS "🟢 clang-tidy added to ${target} target.")
endfunction()