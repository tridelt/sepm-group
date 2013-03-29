set(PLUGIN_NAME "hello_world")
set(PLUGIN_SRCS hello.cpp)


################################################################################
############################     don't edit below     ##########################

add_library("plugin_${PLUGIN_NAME}" SHARED ${PLUGIN_SRCS})

add_custom_command(OUTPUT "copy_plugin_${PLUGIN_NAME}"
    POST_BUILD
    COMMAND cmake -E copy "${CMAKE_CURRENT_BINARY_DIR}/libplugin_${PLUGIN_NAME}.so" "${CMAKE_BINARY_DIR}/bin/plugins/")

add_custom_target("post_build_plugin_${PLUGIN_NAME}" ALL DEPENDS post_build_steps "plugin_${PLUGIN_NAME}" "copy_plugin_${PLUGIN_NAME}")
