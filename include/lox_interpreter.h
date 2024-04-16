#pragma once
#include <string>

namespace Interpreter{
   class LoxInterpreter {
      private:
         static void report_error(int line, std::string where, std::string message);
      public:
         static bool hadError;
         static void error(int line, std::string message) {
            hadError = true; 
            report_error(line, "", message); 
         }
         
   };
}