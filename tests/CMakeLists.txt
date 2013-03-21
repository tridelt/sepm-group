add_custom_target(test)
macro(run_test test_target)
  add_custom_target(${test_target}_runtest
      COMMAND ${test_target} #cmake 2.6 required
      DEPENDS ${test_target}
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
  add_dependencies(test ${test_target}_runtest)
endmacro()


add_executable(basicTests
    basic_tests.cpp
    more_tests.cpp
)
target_link_libraries(basicTests gtest gtest_main pthread)



run_test(basicTests)
