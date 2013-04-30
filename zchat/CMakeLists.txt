add_executable(zchat zchat.cpp backward.cpp IceServer.cpp InterServerImpl.cpp SocketHandler.cpp ChatDB.cpp MessageReader.cpp MessageRouter.cpp ${out_file} ${copy_deps})

set(LIBS common ${LIBS})
target_link_libraries(zchat ${LIBS})
target_link_libraries(zchat common)
target_link_libraries(zchat ice)
target_link_libraries(zchat zmqpp)
