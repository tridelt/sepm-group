add_definitions(-D__SKIP_GNU)   # for ow-crypt (don't redeclare crypt(3))

add_library(server_lib SHARED backward.cpp IceServer.cpp AuthenticationImpl.cpp SessionImpl.cpp PwdHash.cpp Chat.cpp ChatManager.cpp SessionManager.cpp)

add_executable(server server.cpp ${out_file} ${copy_deps})

add_dependencies(common ice)
add_dependencies(common soci-3-2-1)
add_dependencies(common ow-crypt)

target_link_libraries(server ${LIBS})
target_link_libraries(server server_lib)
target_link_libraries(server common)
target_link_libraries(server ice)
target_link_libraries(server ow-crypt)
