add_executable(client client.cpp backward.cpp  ${copy_deps})

set(LIBS ${LIBS})
target_link_libraries(client common)
target_link_libraries(client ice)
target_link_libraries(client ${LIBS})
