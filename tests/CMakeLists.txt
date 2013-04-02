add_custom_target(test)

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(plugins)

add_dependencies(test test_common test_client test_server test_plugins)

