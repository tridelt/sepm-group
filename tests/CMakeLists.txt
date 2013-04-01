add_custom_target(test)

# add test files here
add_executable(basicTests
    basic_tests.cpp
    more_tests.cpp
)

target_link_libraries(basicTests gtest gtest_main pthread ${LIBS})

add_custom_target(basicTests_runtest
    COMMAND basicTests --gtest_output=xml:reports/ #cmake 2.6 required
    DEPENDS basicTests
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tests")


add_subdirectory(plugins)

add_dependencies(test basicTests_runtest plugin_tests)

