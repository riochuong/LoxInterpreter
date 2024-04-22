#pragma once
#include <string>

#include <lexer.h>

namespace Interpreter{
   class LoxInterpreter {
      private:
         static void report_error(int line, std::string where, std::string message);
      public:
         static bool hadError;
         static void error(int line, std::string message) {
            report_error(line, " ", message);
         }
         static void error(Token token, std::string message) {
            hadError = true;
            if (token.type == EndOfFile){
               report_error(token.line, " at end", message);
               return;
            }
            report_error(token.line, " at '" + token.lexeme + "'", message); 
         }
         
   };
}