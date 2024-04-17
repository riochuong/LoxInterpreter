#pragma once

#include <vector>
#include <memory>

#include <expr.h>
#include <token_type.h>

namespace Interpreter{
    class RecursiveDescentParser {
        RecursiveDescentParser(std::vector<Token> tokens): tokens_(tokens) {}

        private:
            const std::vector<Token> tokens_;
            int current_ = 0;

            // private helpers function 
            bool match(std::initializer_list<TokenType> types);
            bool check(TokenType type);
            Token advance();
            bool peek();
            bool is_at_end();
            Token previous();

            // grammar rules 
            std::shared_ptr<Expr> comparison();
            std::shared_ptr<Expr> expressions();
            std::shared_ptr<Expr> equality();
            std::shared_ptr<Expr> term();
            std::shared_ptr<Expr> factor();
            std::shared_ptr<Expr> unary();
            std::shared_ptr<Expr> primary();


    };
}