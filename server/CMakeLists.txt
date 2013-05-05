add_executable(server server.cpp backward.cpp IceServer.cpp AuthenticationImpl.cpp SessionImpl.cpp DBPool.cpp ${out_file} ${copy_deps})

set(LIBS common ${LIBS})
target_link_libraries(server ${LIBS})
target_link_libraries(server common)
target_link_libraries(server ice)
target_link_libraries(server soci_core)
target_link_libraries(server soci_sqlite3)
