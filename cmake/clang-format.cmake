file(GLOB_RECURSE ALL_CXX_SOURCE_FILES *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI])

find_program(CLANG_FORMAT "clang-format")
if(NOT CLANG_FORMAT)
  message(WARNING "🔴 clang-format is not found!")
else()
  message(STATUS "🟢 clang-format found, 'format' target created.")
  add_custom_target(
    format
    COMMAND clang-format
    -i
    -style=file
    ${ALL_CXX_SOURCE_FILES}
    )
endif()
