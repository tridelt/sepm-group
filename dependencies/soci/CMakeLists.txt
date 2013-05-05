include(ExternalProject)
ExternalProject_Add(
   soci-3-2-1
   URL ${CMAKE_CURRENT_SOURCE_DIR}/soci-3.2.1.tar.gz
   CMAKE_ARGS
    -DWITH_SQLITE3=ON
    -DWITH_POSTGRESQL=ON
    -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/soci-3-2-1-prefix
    -DSOCI_USE_BOOST=ON
   BUILD_COMMAND make -j 8
)


include_directories(${CMAKE_CURRENT_BINARY_DIR}/soci-3-2-1-prefix/include)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/soci-3-2-1-prefix/lib)
