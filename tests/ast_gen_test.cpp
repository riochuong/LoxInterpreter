#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <gen_syntax_tree.h>
#include <filesystem>
#include "common.h"


using namespace Interpreter;
namespace fs = std::filesystem;

TEST(ASTGenerationTest, BasicASTGenerationTest){
    std::vector<std::string> types {
         "Binary   : Expr left, Token op, Expr right",
         "Grouping : Expr expression",
         "Literal  : Object value",
         "Unary    : Token op, Expr right"
    };
    std::string basename = "Expr";
    define_abstract_syntax_tree(basename, types, ".");
    EXPECT_TRUE(fs::exists("./" + basename + ".cpp"));
}


int main(int argc, char** argv) {
    ::testing::AddGlobalTestEnvironment(new GlobalSetup);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}