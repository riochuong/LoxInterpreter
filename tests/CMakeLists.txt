# fetch and install gtest 
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip 
)
FetchContent_MakeAvailable(googletest)

# add all tests here 
add_executable(lexer_parser_test lexer_parser_test.cpp)
#add_executable(ast_gen_test ast_gen_test.cpp)
target_link_libraries(lexer_parser_test GTest::gtest_main lexer_lib)
#target_link_libraries(ast_gen_test GTest::gtest_main gen_syntax_tree_lib)

configure_file(assets/basic_lexer_token_read_test.txt ${CMAKE_BINARY_DIR}/tests/basic_lexer_token_read_test.txt COPYONLY)

# Add the test target
add_test(NAME lexer_parser_test COMMAND lexer_parser_test)
#add_test(NAME ast_gen_test COMMAND ast_gen_test)
