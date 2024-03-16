find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


add_executable(lexer_test lexer_test.cpp)
target_link_libraries(lexer_test ${GTEST_LIBRARIES} lexer_lib)


# Add the test target
add_test(NAME lexer_test COMMAND lexer_test)