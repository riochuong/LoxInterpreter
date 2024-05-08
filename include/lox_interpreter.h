#pragma once
#include <string>
#include <expr.h>
#include <lexer.h>

namespace Interpreter{
   class RuntimeErrors: public std::exception {
      public:
         const Token token;
         const std::string message;
         RuntimeErrors(const std::string& msg, const Token &token): message(msg), token(token){}
         const char* what() const noexcept override {
            return message.c_str(); 
         }
   };

   class LoxInterpreter: public Visitor<std::any> {
      private:
         static void report_error(int line, std::string where, std::string message);
         static void report_error(RuntimeErrors error);
         static void runtime_error(RuntimeErrors error);
         std::any evaluate(const Expr& expr);
         bool is_truthy(std::any val);
         bool is_equal(std::any a, std::any b);
         void check_number_operand(Token op, std::any operand);
         void check_number_operands(Token op, std::any left, std::any right);
         std::string stringify(std::any value);
      public:
         std::any visit_binary_expr(const Binary& expr) override;
         std::any visit_grouping_expr(const Grouping& expr) override;
         std::any visit_literal_expr(const Literal& expr) override;
         std::any visit_unary_expr(const Unary& expr) override;
         static bool had_runtime_error;
         static void error(int line, std::string message) {
            report_error(line, " ", message);
         }
         static void error(Token token, std::string message) {
            had_runtime_error = true;
            if (token.type == EndOfFile){
               report_error(token.line, " at end", message);
               return;
            }
            report_error(token.line, " at '" + token.lexeme + "'", message); 
         }
         void interpret(const Expr& expression);
         
   };
}