set(PLUGIN_NAME "hello_world")
set(PLUGIN_SRCS plugin_test.cpp)


################################################################################
############################     don't edit below     ##########################


add_executable(${PLUGIN_NAME}_tests
    ${PLUGIN_SRCS}
)

target_link_libraries(${PLUGIN_NAME}_tests gtest gtest_main pthread ${PLUGIN_NAME})

add_custom_target(${PLUGIN_NAME}_tests_run
    COMMAND ${PLUGIN_NAME}_tests --gtest_output=xml:../../reports/ #cmake 2.6 required
    DEPENDS ${PLUGIN_NAME}_tests
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
