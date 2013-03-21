add_library(gtest "${cxx_strict}" gtest-all.cc)
add_library(gtest_main "${cxx_strict}" gtest_main.cc)
target_link_libraries(gtest_main gtest)
