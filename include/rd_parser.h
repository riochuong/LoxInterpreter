#pragma once

#include <vector>
#include <memory>

#include <expr.h>
#include <token_type.h>

namespace Interpreter{
    class ParseError;
    class RecursiveDescentParser {
        public:
            RecursiveDescentParser(std::vector<Token> tokens): tokens_(tokens) {}
            std::shared_ptr<Expr> parse();

        private:
            const std::vector<Token> tokens_;
            int current_ = 0;

            // private helpers function 
            bool match(std::initializer_list<TokenType> types);
            bool check(TokenType type);
            Token advance();
            Token peek();
            bool is_at_end();
            Token previous();
            Token consume(TokenType type, std::string error_msg);
            ParseError error(Token token, std::string msg);

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