add_executable(client client.cpp ${out_file} ${copy_deps})

set(LIBS common ${LIBS})
target_link_libraries(client ${LIBS})
