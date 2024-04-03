#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <gen_syntax_tree.h>
#include <filesystem>
#include <cstdlib>
#include "common.h"


using namespace Interpreter;
namespace fs = std::filesystem;

// all tests are executed in tests folder so include need to go 2 levels up 
std::string CHECK_FILE_SYNTAX_CMD {"/usr/bin/clang -std=c++17 -I ../../include -Wall -Wextra -fsyntax-only"};

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
    auto cmd = CHECK_FILE_SYNTAX_CMD + " ./" + basename + ".cpp";
    // basic testing to make sure this file is syntactically correct 
    EXPECT_TRUE(!system(cmd.c_str()));
}


int main(int argc, char** argv) {
    ::testing::AddGlobalTestEnvironment(new GlobalSetup);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}