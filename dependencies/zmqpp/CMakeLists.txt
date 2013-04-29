
include_directories(${CMAKE_BINARY_DIR}/dependencies/zmq/zmq-3-2-2-prefix/include)
link_directories(${CMAKE_BINARY_DIR}/dependencies/zmq/zmq-3-2-2-prefix/lib)

add_definitions(-DBUILD_VERSION="2.2.0")
add_definitions(-DBUILD_VERSION_MAJOR=2)
add_definitions(-DBUILD_VERSION_MINOR=2)
add_definitions(-DBUILD_VERSION_REVISION=0)

add_library(zmqpp "${cxx_strict}" message.cpp poller.cpp socket.cpp zmqpp.cpp)
add_dependencies(zmqpp zmq-3-2-2)

target_link_libraries(zmqpp zmq)
