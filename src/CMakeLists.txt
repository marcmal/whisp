add_subdirectory(app)

add_executable(fictional-telegram main.cpp)
target_link_libraries(fictional-telegram PRIVATE app)
