include(ExternalProject)
ExternalProject_Add(
   zmq-3-2-2
   URL ${CMAKE_CURRENT_SOURCE_DIR}/zeromq-3.2.2.tar.gz
   CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
   BUILD_COMMAND make -j 8
)


include_directories(${CMAKE_CURRENT_BINARY_DIR}/zmq-3-2-2-prefix/include)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/zmq-3-2-2-prefix/lib)
