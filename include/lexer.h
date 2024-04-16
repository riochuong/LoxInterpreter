#pragma once
#include <vector>
#include <string>
#include <token_type.h>
#include <any>

namespace Interpreter
{

    class Lexer {
    public:
        std::vector<Token> scan_tokens();
        bool is_at_end();
        Lexer(std::string source) : source_(source) {}

    private:
        std::vector<Token> tokens_;
        std::string source_;
        int start_ = 0;
        int current_ = 0;
        int line_ = 1;

        void scan_token();
        void add_token(TokenType token_type);
        void add_token(TokenType token_type, std::any literal);
        bool match(char c);
        char peek();
        char peek_next();
        void string();
        void number();
        void identifier();
    };
}