# add test files here
add_executable(test_common_exec
    basic_tests.cpp
    more_tests.cpp
)

target_link_libraries(test_common_exec gtest gtest_main pthread ${LIBS})

add_custom_target(test_common
    COMMAND test_common_exec --gtest_output=xml:../reports/
    DEPENDS test_common_exec
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
