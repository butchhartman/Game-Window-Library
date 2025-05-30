add_executable(GameWindowDriver main.c)
target_link_libraries(GameWindowDriver ${PROJECT_NAME})

add_test(GameWindowLinkTest GameWindowDriver)