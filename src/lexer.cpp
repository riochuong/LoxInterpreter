#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include <spdlog/spdlog.h>

#include <lox_interpreter.h>
#include <lexer.h>

using namespace Interpreter;

static bool is_digit(char c){
    return (c >= '0') && (c <= '9');
}

static bool is_alpha(char c) {
    return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
}

bool Lexer::is_at_end(){
    return this->current_ >= this->source_.size();
}

void Lexer::scan_token(){
    char c = source_[current_++]; 
    switch(c){
        case '(': add_token(LEFT_PAREN); break;
        case ')': add_token(RIGHT_PAREN); break;
        case '{': add_token(LEFT_BRACE); break;
        case '}': add_token(RIGHT_BRACE); break;
        case ',': add_token(COMMA); break;
        case '.': add_token(DOT); break;
        case '-': add_token(MINUS); break;
        case '+': add_token(PLUS); break;
        case ';': add_token(SEMICOLON); break;
        case '*': add_token(STAR); break;
        // Need to look at the 2nd character for operators
        case '!':
            add_token(match('=') ? BANG_EQUAL : BANG);
            break;
        case '=':
            add_token(match('=') ? EQUAL_EQUAL : EQUAL);
            break;
        case '<':
            add_token(match('=') ? LESS_EQUAL: LESS);
            break;
        case '>':
            add_token(match('=') ? GREATER_EQUAL : GREATER);
            break;
        case '/':
            // comment go to end of line 
            if (match('/')){
                while((peek() != '\n') && !is_at_end()){
                    current_++;
                }
            } else{
                add_token(SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line_++;
            break;
        case '"': string(); break;
        default: 
            if (is_digit(c)){
                number();
                break;
            } else if(is_alpha(c)){
                identifier();
                break;
            }
            LoxInterpreter::error(line_, "Unexpected character"); 
    }
}

void Lexer::identifier() {
    while(is_alpha(peek()) || is_digit(peek())){
        current_++;
    }
    TokenType type = IDENTIFIER;
    std::string text { source_.substr(start_, current_ - start_)};
    if (STRING_TO_TOKEN_MAP.find(text) != STRING_TO_TOKEN_MAP.end()){
        type = STRING_TO_TOKEN_MAP.at(text);
    }
    add_token(type);
}

void Lexer::number(){
    while(!is_at_end() && is_digit(peek())){
        current_++;
    }
    if (peek() == '.' && is_digit(peek_next())){
        while(!is_at_end() && is_digit(peek())){
            current_++;
        }
    }
    add_token(NUMBER, std::stod(source_.substr(start_, current_ - start_)));
}

void Lexer::string(){
    while(!is_at_end() && peek() != '"'){
        current_++;
    }
    std::string literal { source_.substr(start_ + 1,  current_ - start_)};
    current_++;
    add_token(STRING, literal);
}

void Lexer::add_token(TokenType token_type){
    add_token(token_type, nullptr);
}

void Lexer::add_token(TokenType token_type, std::any literal){
    std::string text { source_.substr(start_, current_ - start_) };
    tokens_.push_back(Token(token_type, text, literal, line_));
}

bool Lexer::match(char expected){
    if (is_at_end()){ return false;}
    if (source_[current_] != expected) return false;
    current_++;
    return true;
}

char Lexer::peek_next(){
    if (current_ + 1 >= source_.size()){
        return '\0';
    }
    return source_[current_ + 1];
}

char Lexer::peek(){
    if (is_at_end()){
        return '\0';
    }
    return source_[current_];
}

std::vector<Token> Lexer::scan_tokens(){

    while(!is_at_end()){
        start_ = current_;
        scan_token();
    }

    tokens_.push_back(Token(EndOfFile, "", nullptr, line_));
    return tokens_;

}