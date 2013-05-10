add_custom_target(test)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(plugins)

add_dependencies(test test_common test_client test_server test_plugins)

