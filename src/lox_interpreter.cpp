#include <iostream>

#include <lox_interpreter.h>

// need to define here so compiler know to allocate storage for it 
bool LoxInterpreter::hadError = false;
    
void LoxInterpreter::report_error(int line, std::string where, std::string message){
    std::cerr << "[" << line << "] Error: " << where << " : " << message << "\n"; 
}