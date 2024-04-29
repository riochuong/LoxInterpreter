#include <gtest/gtest.h>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <utility>
#include <vector>

#include <lexer.h>
#include <rd_parser.h>
#include <ast_printer.h>
#include "common.h"

using namespace Interpreter;

// TODO : FINISH THIS TEST 
TEST(RdParserBasicExpressionTest, TestBasicExpressionsParsing) {
    std::vector<std::string> sources {
        "2 + 3 + 4", 
        "(3 + 4 + 12 +15) <= 100", 
        "!(\"abc\" == \"ABC\")",
        "(2 * 3) + (4 / 2) <= 100 - 5 - 4",
        "!true == false == !true"
    };
    for (auto& s: sources){
        Lexer lexer { s };
        auto tokens = lexer.scan_tokens();
        EXPECT_TRUE(!tokens.empty());
        RecursiveDescentParser parser {tokens};
        auto expr = parser.parse();
        AstPrinter printer;
        EXPECT_TRUE(expr != nullptr);
        spdlog::info("Expr: {}", printer.print(*expr));
        expr = parser.parse();
    }
}

int main(int argc, char** argv) {
    ::testing::AddGlobalTestEnvironment(new GlobalSetup);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}