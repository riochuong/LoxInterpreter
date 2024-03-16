#include <iostream>
#include <fstream>
#include <sstream>
#include <lexer.h>
#include <cerrno>
#include <cstring>
#include <spdlog/spdlog.h>

std::vector<std::string> Lexer::read_all_token_from_file(std::string filepath){

    std::vector<std::string> tokens; 
    std::ifstream input_file (filepath, std::ios::in);
    if (!input_file.is_open()){
        spdlog::error("Failed to open file with error %s", strerror(errno));
        return tokens;
    }

    std::string line;
    while(std::getline(input_file, line)){
        std::stringstream ss(line);
        std::string token;
        while(std::getline(ss, token, ' ')){
            //remove_if move all non_whitespaces to front and return the pointer to the last 
            // white space that can be remove
            token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
            if (token.size()){
                tokens.push_back(token);
            }
        }
    }
    input_file.close();
    return tokens;
}