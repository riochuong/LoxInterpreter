#include <ast_printer.h>

using namespace Interpreter;


std::any AstPrinter::visit_binary_expr(const Binary &expr) {
    std::vector<std::shared_ptr<Expr>> exprs = {expr.left , expr.right};
    return parenthesize(expr.op.lexeme, exprs);
}

std::any AstPrinter::visit_grouping_expr(const Grouping &expr){
    std::vector<std::shared_ptr<Expr>> exprs = {expr.expression};
    return parenthesize("group", exprs);        
}

std::any AstPrinter::visit_literal_expr(const Literal &expr){
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

std::any AstPrinter::visit_unary_expr(const Unary &expr){
    std::vector<std::shared_ptr<Expr>> exprs = {expr.right}; 
    return parenthesize(expr.op.lexeme, exprs);
}

std::string AstPrinter::parenthesize(std::string name, std::vector<std::shared_ptr<Expr>> exprs){
    std::string res = "(" +name;
    for (auto& expr: exprs){
        res += " ";
        res += std::any_cast<std::string>(expr->accept(*this));
    }
    res += ")";
    return res;
}
    
std::string AstPrinter::print(const Expr& expr){
    return std::any_cast<std::string>(expr.accept(*this));
}


