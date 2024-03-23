#include <gtest/gtest.h>
#include <lexer.h>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <utility>
#include <vector>

#define TEST_LOGGER "gtest_logger"
auto LOG_LEVEL = spdlog::level::debug;

class GlobalSetup : public ::testing::Environment {
public:
    void SetUp() override {
        spdlog::set_level(LOG_LEVEL);
    }

    void TearDown() override {
       
    }
};

TEST(LexerBasicTest, SimpleVariableAssignment) {
    Lexer lexer {"var a = 123; // this is the basic test "};
    auto tokens = lexer.scan_tokens();
    for (auto &token: tokens){
        spdlog::debug("{}", token.lexeme);
    }
    EXPECT_EQ(tokens[0].lexeme, "var");
    EXPECT_EQ(tokens[0].type, VAR);
    
    EXPECT_EQ(tokens[1].lexeme, "a");
    EXPECT_EQ(tokens[1].type, IDENTIFIER);

    EXPECT_EQ(tokens[2].lexeme, "=");
    EXPECT_EQ(tokens[2].type, EQUAL);
 
    EXPECT_EQ(tokens[3].lexeme, "123");
    EXPECT_EQ(tokens[3].type, NUMBER);
    EXPECT_TRUE((std::any_cast<double>(tokens[3].literal) - 123) < 0.00001);

    EXPECT_EQ(tokens[4].lexeme, ";");
    EXPECT_EQ(tokens[4].type, SEMICOLON);

    EXPECT_EQ(tokens[5].lexeme, "");
    EXPECT_EQ(tokens[5].type, EndOfFile);

}

TEST(LexerBasicTest, ForLoopBasic) {
    
    auto source = R"(
        a = 1;
        limit = 10;
        b = 0;
        for(a = 0; a < limit; a = a + 1){
            b = b + 1;
        })";
    std::vector<std::pair<std::string, TokenType>> expected_results = {
        {"a",       IDENTIFIER},
        {"=",       EQUAL},
        {"1",       NUMBER},
        {";",       SEMICOLON},
        {"limit",   IDENTIFIER},
        {"=",       EQUAL},
        {"10",      NUMBER},
        {";",       SEMICOLON},
        {"b",       IDENTIFIER},
        {"=",       EQUAL},
        {"0",       NUMBER},
        {";",       SEMICOLON},
        {"for",      FOR},
        {"(",      LEFT_PAREN},
        {"a",      IDENTIFIER},
        {"=",      EQUAL}, 
        {"0",      NUMBER},
        {";",       SEMICOLON},
        {"a",      IDENTIFIER},
        {"<",      LESS},
        {"limit",      IDENTIFIER},
        {";",       SEMICOLON},
        {"a",      IDENTIFIER},
        {"=",      EQUAL},
        {"a",      IDENTIFIER},
        {"+",      PLUS},
        {"1",      NUMBER},
        {")",      RIGHT_PAREN},
        {"{",      LEFT_BRACE},
        {"b",      IDENTIFIER},
        {"=",      EQUAL},
        {"b",      IDENTIFIER},
        {"+",      PLUS},
        {"1",      NUMBER},
        {";",       SEMICOLON},
        {"}",      RIGHT_BRACE},
        {"",      EndOfFile},
    };
    Lexer lexer { source };
    auto tokens = lexer.scan_tokens();
    for (auto &token: tokens){
        spdlog::debug("{}", token.lexeme);
    }
    EXPECT_EQ(tokens.size(), expected_results.size());
    for (int i = 0; i < tokens.size(); i++){
        spdlog::debug("Item {} lexeme expected {} received {}", i, tokens[i].lexeme, expected_results.at(i).first);
        spdlog::debug("Item {} type expected {} received {}", i, tokens[i].type, expected_results.at(i).second);
        EXPECT_EQ(tokens[i].lexeme, expected_results.at(i).first);
        EXPECT_EQ(tokens[i].type, expected_results.at(i).second);
    }
}

int main(int argc, char** argv) {
    ::testing::AddGlobalTestEnvironment(new GlobalSetup);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}