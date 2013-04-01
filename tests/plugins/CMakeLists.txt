add_custom_target(plugin_tests)

add_subdirectory(hello_world)

add_dependencies(plugin_tests hello_world_tests_run)
