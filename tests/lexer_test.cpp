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

static void verify_lexer_from_expected_list(std::vector<Token> tokens, std::vector<std::pair<std::string, TokenType>> expected_results){
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
    verify_lexer_from_expected_list(tokens, expected_results);
    
    // for (auto &token: tokens){
    //     spdlog::debug("{}", token.lexeme);
    // }
    // EXPECT_EQ(tokens.size(), expected_results.size());
    // for (int i = 0; i < tokens.size(); i++){
    //     spdlog::debug("Item {} lexeme expected {} received {}", i, tokens[i].lexeme, expected_results.at(i).first);
    //     spdlog::debug("Item {} type expected {} received {}", i, tokens[i].type, expected_results.at(i).second);
    //     EXPECT_EQ(tokens[i].lexeme, expected_results.at(i).first);
    //     EXPECT_EQ(tokens[i].type, expected_results.at(i).second);
    // }
}


TEST(LexerBasicTest, WhileLoopBasic) {
    auto source = R"(
        var a = 1;
        var b = 2;
        var c = 0;
        while(a <= 10){
            a = a + 1;
            if b != 0 {
                a = a + 2;
            } else {
                a = a * 1;
            }
            if c >= 0{
                c = c - 1;
            }
            if !(a == b) or (b <= c){
                print("test");
            } 
        }
    )";

    

    std::vector<std::pair<std::string, TokenType>> expected_results = {
        {"var",       VAR},
        {"a",       IDENTIFIER},
        {"=",       EQUAL},
        {"1",       NUMBER},
        {";",       SEMICOLON},
        {"var",       VAR},
        {"b",   IDENTIFIER},
        {"=",       EQUAL},
        {"2",      NUMBER},
        {";",       SEMICOLON},
        {"var",       VAR},
        {"c",       IDENTIFIER},
        {"=",       EQUAL},
        {"0",       NUMBER},
        {";",       SEMICOLON},
        {"while",      WHILE},
        {"(",      LEFT_PAREN},
        {"a",      IDENTIFIER},
        {"<=",      LESS_EQUAL}, 
        {"10",      NUMBER},
        {")",       RIGHT_PAREN},
        {"{",      LEFT_BRACE},
        {"a",      IDENTIFIER},
        {"=",      EQUAL},
        {"a",       IDENTIFIER},
        {"+",      PLUS},
        {"1",      NUMBER},
        {";",      SEMICOLON},
        {"if",      IF},
        {"b",      IDENTIFIER},
        {"!=",      BANG_EQUAL},
        {"0",      NUMBER},
        {"{",      LEFT_BRACE},
        {"a",      IDENTIFIER},
        {"=",      EQUAL},
        {"a",      IDENTIFIER},
        {"+",      PLUS},
        {"2",       NUMBER},
        {";",      SEMICOLON},
        {"}",      RIGHT_BRACE},
        {"else",      ELSE},
        {"{",      LEFT_BRACE},
        {"a",      IDENTIFIER},
        {"=",      EQUAL},
        {"a",      IDENTIFIER},
        {"*",      STAR},
        {"1",      NUMBER},
        {";",      SEMICOLON},
        {"}",      RIGHT_BRACE},
        {"if",      IF},
        {"c",      IDENTIFIER},
        {">=",      GREATER_EQUAL},
        {"0",      NUMBER},
        {"{",      LEFT_BRACE},
        {"c",      IDENTIFIER},
        {"=",      EQUAL},
        {"c",      IDENTIFIER},
        {"-",      MINUS},
        {"1",      NUMBER},
        {";",      SEMICOLON},
        {"}",      RIGHT_BRACE},
        {"if",      IF},
        {"!",      BANG},
        {"(",      LEFT_PAREN},
        {"a",      IDENTIFIER},
        {"==",      EQUAL_EQUAL},
        {"b",      IDENTIFIER},
        {")",      RIGHT_PAREN},
        {"or",      OR},
        {"(",      LEFT_PAREN},
        {"b",      IDENTIFIER},
        {"<=",      LESS_EQUAL},
        {"c",      IDENTIFIER},
        {")",      RIGHT_PAREN},
        {"{",      LEFT_BRACE},
        {"print",      PRINT},
        {"(",      LEFT_PAREN},
        {"\"test\"",     STRING},
        {")",     RIGHT_PAREN},
        {";",     SEMICOLON},
        {"}",      RIGHT_BRACE},
        {"}",      RIGHT_BRACE},
        {"",      EndOfFile},
    };

    Lexer lexer { source };
    auto tokens = lexer.scan_tokens();
    verify_lexer_from_expected_list(tokens, expected_results);
}

TEST(LexerBasicTest, BasicClassDeclaration) {
    auto source = R"(
        class Parent {
            
        };
    })";
}

int main(int argc, char** argv) {
    ::testing::AddGlobalTestEnvironment(new GlobalSetup);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}