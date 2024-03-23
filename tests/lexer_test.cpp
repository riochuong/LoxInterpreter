#include <gtest/gtest.h>
#include <lexer.h>
#include <filesystem>
#include <spdlog/spdlog.h>

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

TEST(LexerBasicTest, ReadTokenFromInputFile) {
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

}

int main(int argc, char** argv) {
    ::testing::AddGlobalTestEnvironment(new GlobalSetup);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}