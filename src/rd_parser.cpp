#include <rd_parser.h>

using namespace Interpreter;

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
    std::shared_ptr<Expr> expr = this->term();
    while(match({LESS_EQUAL, LESS, GREATER_EQUAL, GREATER})){
        Token op = this->previous();
        std::shared_ptr<Expr> right = this->term();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> RecursiveDescentParser::factor(){
    std::shared_ptr<Expr> expr = this->factor();
    while(match({MINUS, PLUS})){
        Token op = this->previous();
        std::shared_ptr<Expr> right = this->factor();
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