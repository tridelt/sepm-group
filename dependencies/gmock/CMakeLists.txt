include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/gtest/include")
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/" "${CMAKE_CURRENT_SOURCE_DIR}/gtest")

add_library(gtest "${cxx_strict}" "${CMAKE_CURRENT_SOURCE_DIR}/src/gmock-all.cc" "${CMAKE_CURRENT_SOURCE_DIR}/gtest/src/gtest-all.cc")


add_library(gtest_main "${cxx_strict}" "${CMAKE_CURRENT_SOURCE_DIR}/gtest/src/gtest_main.cc")

target_link_libraries(gtest_main gtest)


