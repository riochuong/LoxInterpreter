#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <any>

namespace Interpreter
{
    enum TokenType
    {
        // Single-character tokens.
        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_BRACE,
        RIGHT_BRACE,
        COMMA,
        DOT,
        MINUS,
        PLUS,
        SEMICOLON,
        SLASH,
        STAR,

        // One or two character tokens.
        BANG,
        BANG_EQUAL,
        EQUAL,
        EQUAL_EQUAL,
        GREATER,
        GREATER_EQUAL,
        LESS,
        LESS_EQUAL,

        // Literals.
        IDENTIFIER,
        STRING,
        NUMBER,

        // Keywords.
        AND,
        CLASS,
        ELSE,
        FALSE,
        FUN,
        FOR,
        IF,
        NIL,
        OR,
        PRINT,
        RETURN,
        SUPER,
        THIS,
        TRUE,
        VAR,
        WHILE,

        EndOfFile
    };

    const std::unordered_map<std::string, TokenType> STRING_TO_TOKEN_MAP = {
        {"and", AND},
        {"class", CLASS},
        {"else", ELSE},
        {"false", FALSE},
        {"for", FOR},
        {"fun", FUN},
        {"if", IF},
        {"nil", NIL},
        {"or", OR},
        {"print", PRINT},
        {"return", RETURN},
        {"super", SUPER},
        {"this", THIS},
        {"true", TRUE},
        {"var", VAR},
        {"while", WHILE}};

    class Token
    {
    public:
        const TokenType type;
        const std::string lexeme;
        const std::any literal;
        const int line;

        Token(TokenType type, std::string lexeme, std::any literal, int line) : type(type),
                                                                                lexeme(lexeme),
                                                                                literal(literal),
                                                                                line(line)
        {

            // TODO: might need to do some type casting here for void*
        }

        Token(const Token &other) : type(other.type),
                                    lexeme(other.lexeme),
                                    literal(other.literal),
                                    line(other.line) {}

        // Overload the << operator to provide a "toString()" functionality
        friend std::ostream &operator<<(std::ostream &os, const Token &obj)
        {
            os << obj.type << " " << obj.lexeme << " "
               << "\n";
            return os;
        }
    };
}