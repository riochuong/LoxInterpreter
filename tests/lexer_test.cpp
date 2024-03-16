#include <gtest/gtest.h>
#include <lexer.h>


TEST(LexerBasicTest, ReadTokenFromInputFile) {
    Lexer lexer;
    auto tokens = lexer.read_all_token_from_file("basic_lexer_token_read_test.txt");
    for (std::string_view token: tokens){
        std::cout << token << "\n";
    }
    EXPECT_EQ(tokens.size(), 21);
}