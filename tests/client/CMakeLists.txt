# add test files here
add_executable(test_client_exec
    basic_tests.cpp
)

target_link_libraries(test_client_exec gtest gtest_main pthread ${LIBS} common)

add_custom_target(test_client
    COMMAND test_client_exec --gtest_output=xml:../reports/
    DEPENDS test_client_exec
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
