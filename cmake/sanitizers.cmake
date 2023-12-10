function(setup_sanitizers target)
  set(SANITIZERS -fsanitize=address -fsanitize=leak -fsanitize=undefined)

  target_compile_options(${target} PUBLIC ${SANITIZERS})
  target_link_options(${target} PUBLIC ${SANITIZERS})
  
  message(STATUS "🟢 configured sanitizers for ${target} target.")
endfunction()
