# fetch and install gtest 
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip 
)
FetchContent_MakeAvailable(googletest)

# add all tests here 
add_executable(lexer_test lexer_test.cpp)
target_link_libraries(lexer_test GTest::gtest_main lexer_lib)

configure_file(assets/basic_lexer_token_read_test.txt ${CMAKE_BINARY_DIR}/tests/basic_lexer_token_read_test.txt COPYONLY)

# Add the test target
add_test(NAME lexer_test COMMAND lexer_test)
