#include <rd_parser.h>
#include <lox_interpreter.h>
#include <exception>

using namespace Interpreter;

class Interpreter::ParseError: std::exception {
    private:
        const std::string msg_ = "ParseError";
    public:
        ParseError(){}
        const char* what() const noexcept override {
            return msg_.c_str();
        }
};

std::shared_ptr<Expr> RecursiveDescentParser::expressions()
{
    return this->equality();
}

std::shared_ptr<Expr> RecursiveDescentParser::equality()
{
    std::shared_ptr<Expr> expr = this->comparison();
    while (match({BANG_EQUAL, EQUAL_EQUAL}))
    {
        Token op = this->previous();
        std::shared_ptr<Expr> right = this->comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> RecursiveDescentParser::comparison(){
    std::shared_ptr<Expr> expr = this->term();
    while(match({LESS_EQUAL, LESS, GREATER_EQUAL, GREATER})){
        Token op = this->previous();
        std::shared_ptr<Expr> right = this->term();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}


std::shared_ptr<Expr> RecursiveDescentParser::term(){
    std::shared_ptr<Expr> expr = this->factor();
    while(match({MINUS, PLUS})){
        Token op = this->previous();
        std::shared_ptr<Expr> right = this->factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> RecursiveDescentParser::factor(){
    std::shared_ptr<Expr> expr = this->unary();
    while(match({STAR, SLASH})){
        Token op = this->previous();
        std::shared_ptr<Expr> right = this->unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> RecursiveDescentParser::unary(){
    if(match({BANG, MINUS})){
        Token op = this->previous();
        return std::make_shared<Unary>(op, this->unary());
    }
    return this->primary();
}

std::shared_ptr<Expr> RecursiveDescentParser::primary(){
    if (match({FALSE})) return std::make_shared<Literal>(false);
    if (match({TRUE})) return std::make_shared<Literal>(true);
    if (match({NIL})) return std::make_shared<Literal>(NULL);

    if (match({NUMBER, STRING})){return std::make_shared<Literal>(previous().literal);}

    if (match({LEFT_PAREN})){
        std::shared_ptr<Expr> expr = expressions();
        consume(RIGHT_PAREN, "Failed to consume right parenthese !");
        return std::make_shared<Grouping>(expr);
    }
    throw error(peek(), "Expect expression but current is lexeme "+peek().lexeme); 
}

bool RecursiveDescentParser::match(std::initializer_list<TokenType> types)
{
    for (auto token_type : types)
    {
        if (check(token_type))
        {
            this->advance();
            return true;
        }
    }
    return false;
}


bool RecursiveDescentParser::is_at_end(void){
    return tokens_.at(current_).type == EndOfFile;
}

Token RecursiveDescentParser::peek(void){
    return tokens_.at(current_);
}

Token RecursiveDescentParser::previous(void){
    return tokens_.at(current_ - 1);
}

Token RecursiveDescentParser::advance(void){
    if(!is_at_end()) { current_++;}
    return previous();
}

Token RecursiveDescentParser::consume(TokenType type, std::string error_msg){
    if (check(type)) { return advance(); }
    throw ParseError();
}


ParseError RecursiveDescentParser::error(Token token, std::string msg){
    LoxInterpreter::error(token, msg);
    return ParseError();
}

bool RecursiveDescentParser::check(TokenType type){
    if (is_at_end()) {return false;}
    return tokens_[current_].type == type;
}

std::shared_ptr<Expr> RecursiveDescentParser::parse(void) {
    try{
        return expressions();
    } catch (const ParseError& e)  {
        return nullptr;
    }
}