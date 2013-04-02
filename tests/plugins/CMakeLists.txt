add_custom_target(test_plugins)

add_subdirectory(hello_world)

add_dependencies(test_plugins test_hello_world)
