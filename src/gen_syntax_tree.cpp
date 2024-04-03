#include <gen_syntax_tree.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <utility>
#include <cctype>
#include <algorithm>

namespace fs = std::filesystem;

static std::string strip(const std::string& s) {
    auto begin = std::find_if(s.begin(), s.end(), [](char ch) {
        return !std::isspace(static_cast<unsigned char>(ch));
    });
    auto end = std::find_if(s.rbegin(), s.rend(), [](char ch) {
        return !std::isspace(static_cast<unsigned char>(ch));
    }).base(); // covert reverse pointer to forward 

    return (begin < end) ? std::string(begin, end) : s;
}

static void tolower_inplace (std::string& s){
    std::for_each(s.begin(), s.end(), [](char& c){c = tolower(c);});
}

std::string Interpreter::define_type(std::string basename, std::string type, std::string field_list){
     std::string res;
     std::string field;
     type = strip(type); 
     std::string constructor = "         " + type + "(";
     std::vector <std::pair<std::string, std::string>> field_pairs;
     std::stringstream ss {field_list};
     res += "    class " + type + " : " + basename + " {" +"\n";
     res += "        public:\n";
     while (std::getline(ss, field, ',')){
            // stripped all spaces
            field = strip(field);
            size_t delim_pos = field.find(' ');
            std::string field_type = strip(field.substr(0, delim_pos));
            std::string field_name = strip(field.substr(delim_pos + 1));
            // need to convert generic Object to std::any for C++ vs Java
            if (field_type == "Object") {
                field_type = "std::any";
            }
            res += "         const " + field_type + "  " + field_name + ";\n";
            field_pairs.emplace_back(field_type, field_name);
     }
     // construct header of constructor
     for (int i = 0; i < field_pairs.size(); i++){
        if (i > 0 && i <= (field_pairs.size() - 1)){
            constructor += ", ";
        }
        constructor += field_pairs[i].first + " " +field_pairs[i].second;
     }
     constructor += "): ";
     // construct header of constructor
     for (int i = 0; i < field_pairs.size(); i++){
        if (i > 0 && i <= (field_pairs.size() -1)){
            constructor += ", ";
        }
        constructor += field_pairs[i].second + "(" + field_pairs[i].second + ")";
     }
     constructor += " {}\n";
     constructor += "    };\n";
     return res + constructor;     
}

void Interpreter::define_abstract_syntax_tree(std::string basename, std::vector<std::string> types, std::string output_dir){
    fs::path output_file = fs::path(output_dir) / (basename + ".cpp") ;
    std::ofstream of {output_file};
    if (!of.is_open()){
        spdlog::error("Failed to open file at {} for output", output_file.string());
    }
    of << "#include <token_type.h>\n\n";
    of << "namespace Interpreter {\n";
    of << "    class " << basename << " {};";
    of << "\n";
    for (auto &type: types){
        size_t delim_pos = type.find(':');
        of << define_type(basename, type.substr(0, delim_pos), type.substr(delim_pos + 1));
    }
    of << "}";
    of.close();
}
