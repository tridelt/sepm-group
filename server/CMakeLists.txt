add_library(server_lib SHARED backward.cpp IceServer.cpp AuthenticationImpl.cpp SessionImpl.cpp)

add_executable(server server.cpp ${out_file} ${copy_deps})

target_link_libraries(server ${LIBS})
target_link_libraries(server server_lib)
target_link_libraries(server common)
target_link_libraries(server ice)
