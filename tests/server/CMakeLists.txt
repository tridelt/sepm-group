# add test files here
add_executable(test_server_exec
    basic_tests.cpp
)

target_link_libraries(test_server_exec gtest gtest_main pthread ${LIBS})

add_custom_target(test_server
    COMMAND test_server_exec --gtest_output=xml:../reports/
    DEPENDS test_server_exec
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
