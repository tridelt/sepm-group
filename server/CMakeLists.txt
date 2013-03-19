add_executable(server server.cpp ${out_file} ${copy_deps})

set(LIBS common ${LIBS})
target_link_libraries(server ${LIBS})
