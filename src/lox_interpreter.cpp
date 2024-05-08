#include <iostream>

#include <lox_interpreter.h>

using namespace Interpreter;

// need to define here so compiler know to allocate storage for it 
bool Interpreter::LoxInterpreter::had_runtime_error = false;
    
void Interpreter::LoxInterpreter::report_error(int line, std::string where, std::string message){
    std::cerr << "[" << line << "] Error: " << where << " : " << message << "\n"; 
}

std::any LoxInterpreter::visit_literal_expr(const Literal &expr){
    return expr.value;
}
std::any LoxInterpreter::visit_grouping_expr(const Grouping &expr){
    return evaluate(*expr.expression);
}


std::any LoxInterpreter::visit_unary_expr(const Unary& expr){
    std::any right = evaluate(*expr.right);

    switch (expr.op.type)
    {
        case MINUS:
            return -(std::any_cast<double>(right));
        case BANG:
            return !is_truthy(right); 
        default:
            throw RuntimeErrors("unsupported unary op", expr.op); 
    };

    return std::any();
}

bool LoxInterpreter::is_truthy(std::any val){
    if (!val.has_value()) { return false; }
    try {
        return std::any_cast<bool>(val);
    } catch (const std::bad_any_cast&) {
        return true;
    }
}

bool LoxInterpreter::is_equal(std::any a, std::any b){
    if (!a.has_value() && !b.has_value()) {return true;}
    if (!a.has_value()){ return false;}
    try{
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    } catch (const std::bad_any_cast&) {}
    try {
        return std::any_cast<double>(a) == std::any_cast<double>(b);
    } catch (const std::bad_any_cast&) {}
    try {
        return std::any_cast<int>(a) == std::any_cast<int>(b);
    } catch (const std::bad_any_cast&) {}
    try {
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    } catch (const std::bad_any_cast&) {}
    throw std::runtime_error("Unsupported Literal Type !!");  
}

std::any LoxInterpreter::visit_binary_expr(const Binary& expr){
    std::any left = evaluate(*expr.left);
    std::any right = evaluate(*expr.right);

    switch(expr.op.type){
        case MINUS:
            check_number_operands(expr.op, left, right);
            auto left_dm = std::any_cast<double>(left);
            auto right_dm = std::any_cast<double>(right);
            return left_dm - right_dm;
        case SLASH:
            check_number_operands(expr.op, left, right);
            auto left_ds = std::any_cast<double>(left);
            auto right_ds = std::any_cast<double>(right);
            return left_ds / right_ds;
        case STAR:
            check_number_operands(expr.op, left, right);
            auto left_dstr = std::any_cast<double>(left);
            auto right_dstr = std::any_cast<double>(right);
            return left_dstr * right_dstr;
        case PLUS:
            try{
                auto left_dp = std::any_cast<double>(left);
                auto right_dp = std::any_cast<double>(right);
                return left_dp + right_dp;
            }catch (const std::bad_any_cast&) {
                try{
                    auto left_s = std::any_cast<std::string>(left);
                    auto right_s = std::any_cast<std::string>(right);
                    return left_s + right_s; 
                } catch (const std::bad_any_cast&){
                   throw RuntimeErrors("Operand must be two strings or two numbers", expr.op);   
                }
            }
            break;
        case GREATER:
            check_number_operands(expr.op, left, right);
            auto left_dg = std::any_cast<double>(left);
            auto right_dg = std::any_cast<double>(right);
            return left_dg > right_dg;
        case GREATER_EQUAL:
            check_number_operands(expr.op, left, right);
            auto left_dge = std::any_cast<double>(left);
            auto right_dge = std::any_cast<double>(right);
            return left_dge >= right_dge;
        case LESS:
            check_number_operands(expr.op, left, right);
            auto left_dl = std::any_cast<double>(left);
            auto right_dl = std::any_cast<double>(right);
            return left_dl < right_dl;
        case LESS_EQUAL:
            check_number_operands(expr.op, left, right);
            auto left_dle = std::any_cast<double>(left);
            auto right_dle = std::any_cast<double>(right);
            return left_dle <= right_dle;
    }

}

std::any LoxInterpreter::evaluate(const Expr& expr){
    return expr.accept(*this);
}


void LoxInterpreter::check_number_operand(Token op, std::any operand){
    try{
        std::any_cast<double>(operand);
    } catch (const std::bad_any_cast&) {
         throw RuntimeErrors("Operand must be a number", op);  
    } 
}

void LoxInterpreter::interpret(const Expr& expression){
    try {
        std::any value = evaluate(expression);
    } catch (RuntimeErrors error){
        LoxInterpreter::runtime_error(error);
    }
}

void LoxInterpreter::runtime_error(RuntimeErrors error){
     LoxInterpreter::had_runtime_error = true;
     std::cout << error.what() << "\n [line " + error.token.line << "]\n";
}

std::string LoxInterpreter::stringify(std::any val){
    if (!val.has_value()) {return "nil";}
    
    // double
    try {
        auto v = std::to_string(std::any_cast<double>(val));
        size_t pos = v.rfind(".0");
        if (pos != std::string::npos && pos == (v.size() - 2)){
            return v.substr(0, pos);
        }
        return v;
    } catch (const std::bad_any_cast&){}

    // bool
    try {
        bool v = std::any_cast<bool>(val);
        return v ? "true" : "false";
    } catch (const std::bad_any_cast&){} 
    
    // string
    try {
        return std::any_cast<std::string>(val);
    } catch (const std::bad_any_cast&){}

    // int
    try {
        return std::to_string(std::any_cast<int>(val));
    } catch (const std::bad_any_cast&){}
    throw std::runtime_error("Unsupported Literal Type for stringify!!");  
}

void LoxInterpreter::check_number_operands(Token op, std::any left, std::any right){
    try{
        std::any_cast<double>(left);
        std::any_cast<double>(right);
    }catch (const std::bad_any_cast&) {
        throw RuntimeErrors("Operand must be a number", op);  
    } 
}
