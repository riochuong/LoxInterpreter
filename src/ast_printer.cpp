#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <expr.h>
#include <rd_parser.h>

using namespace Interpreter;

class AstPrinter: Visitor<std::any> {

    std::any visit_binary_expr(const Binary &expr) {
        std::vector<std::shared_ptr<Expr>> exprs = {expr.left , expr.right};
        return parenthesize(expr.op.lexeme, exprs);
    }

    std::any visit_grouping_expr(const Grouping &expr){
        std::vector<std::shared_ptr<Expr>> exprs = {expr.expression};
        return parenthesize("group", exprs);        
    }

    std::any visit_literal_expr(const Literal &expr){
        if (!expr.value.has_value()) { return "nil"; }
        try{
            return std::any_cast<std::string>(expr.value);
        } catch (const std::bad_any_cast&) {}
        try {
            return std::to_string(std::any_cast<double>(expr.value));
        } catch (const std::bad_any_cast&) {}
        try {
            return std::to_string(std::any_cast<int>(expr.value));
        } catch (const std::bad_any_cast&) {}
        throw std::runtime_error("Unsupported Literal Type !!"); 


    }    

    std::any visit_unary_expr(const Unary &expr){
       std::vector<std::shared_ptr<Expr>> exprs = {expr.right}; 
       return parenthesize(expr.op.lexeme, exprs);
    }

    std::string parenthesize(std::string name, std::vector<std::shared_ptr<Expr>> exprs){
        std::string res = "(" +name;
        for (auto& expr: exprs){
            res += " ";
            res += std::any_cast<std::string>(expr->accept(*this));
        }
        res += ")";
        return res;
    }
    public:
        std::string print(const Expr& expr){
            return std::any_cast<std::string>(expr.accept(*this));
        }

};

int main(void) {
    AstPrinter printer;
    auto expression = std::make_shared<Binary>(
                    std::make_shared<Unary>(Token(MINUS, "-", nullptr, 1), std::make_shared<Literal>(123)), 
                    Token(STAR, "*", nullptr, 1),
                    std::make_shared<Grouping>(std::make_shared<Literal>(45.67)));
                   

    std::cout << printer.print(*expression) << std::endl; 
    return 0;
}