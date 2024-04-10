#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <expr.h>

using namespace Interpreter;

class AstPrinter: Visitor<std::string> {

    std::string visit_binary_expr(Binary<std::string> *expr){
        std::vector<std::shared_ptr<Expr<std::string>>> exprs = {expr->left , expr->right};
        return parenthesize(expr->op.lexeme, exprs);
    }

    std::string visit_grouping_expr(Grouping<std::string> *expr){
        std::vector<std::shared_ptr<Expr<std::string>>> exprs = {expr->expression};
        return parenthesize("group", exprs);        
    }

    std::string visit_literal_expr(Literal<std::string> *expr){
        if (!expr->value.has_value()) { return "nil"; }
        try{
            return std::any_cast<std::string>(expr->value);
        } catch (const std::bad_any_cast&) {}
        try {
            return std::to_string(std::any_cast<double>(expr->value));
        } catch (const std::bad_any_cast&) {}
        try {
            return std::to_string(std::any_cast<int>(expr->value));
        } catch (const std::bad_any_cast&) {}
        throw std::runtime_error("Unsupported Literal Type !!"); 


    }    

    std::string visit_unary_expr(Unary<std::string> *expr){
       std::vector<std::shared_ptr<Expr<std::string>>> exprs = {expr->right}; 
       return parenthesize(expr->op.lexeme, exprs);
    }

    std::string parenthesize(std::string name, std::vector<std::shared_ptr<Expr<std::string>>> exprs){
        std::string res = "(" +name;
        for (auto& expr: exprs){
            res += " ";
            res += expr->accept(this);
        }
        res += ")";
        return res;
    }
    public:
        std::string print(Expr<std::string>* expr){
            return expr->accept(this);
        }

};

int main(void) {
    AstPrinter printer;
    auto expression = std::make_shared<Binary<std::string>>(
                    std::make_shared<Unary<std::string>>(Token(MINUS, "-", nullptr, 1), std::make_shared<Literal<std::string>>(123)), 
                    Token(STAR, "*", nullptr, 1),
                    std::make_shared<Grouping<std::string>>(std::make_shared<Literal<std::string>>(45.67)));

    std::cout << printer.print(expression.get()) << std::endl; 
    return 0;
}