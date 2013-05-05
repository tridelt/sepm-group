set(PLUGIN_NAME "hello_world")
set(PLUGIN_SRCS hello.cpp)


################################################################################
############################     don't edit below     ##########################

add_library("${PLUGIN_NAME}" SHARED ${PLUGIN_SRCS})

target_link_libraries(${PLUGIN_NAME} ${LIBS})
target_link_libraries(${PLUGIN_NAME} common)

add_custom_command(OUTPUT "copy_plugin_${PLUGIN_NAME}"
    POST_BUILD
    COMMAND cmake -E copy "${CMAKE_CURRENT_BINARY_DIR}/lib${PLUGIN_NAME}.so" "${CMAKE_BINARY_DIR}/bin/plugins/")

add_custom_target("post_build_plugin_${PLUGIN_NAME}" ALL DEPENDS post_build_steps "${PLUGIN_NAME}" "copy_plugin_${PLUGIN_NAME}")
