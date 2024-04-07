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

std::string Interpreter::define_visitor(std::string basename, std::vector<std::string> types){
    std::string visitor_str = ""; 
    visitor_str += "    template<typename R>\n";
    visitor_str += "        class Visitor{\n";
    for(auto& type: types){
        size_t delim_pos = type.find(":");
        basename = strip(basename);
        std::string type_str {strip(type.substr(0, delim_pos))};
        std::string type_str_lower {strip(type.substr(0, delim_pos))};
        tolower_inplace(type_str_lower);
        tolower_inplace(basename);
        visitor_str += "            virtual R visit_" + type_str_lower + "_" + basename + "(";
        visitor_str += type_str + "<R>* " + basename +");\n"; 
    }
    visitor_str += "        };\n";
    return visitor_str;
}

std::string Interpreter::define_type(std::string basename, std::string type, std::string field_list){
     std::string res;
     std::string field;
     type = strip(type); 
     std::string constructor = "         " + type + "(";
     std::string func = "         R accept(Visitor<R> visitor){\n";
     std::vector <std::pair<std::string, std::string>> field_pairs;
     std::stringstream ss {field_list};
     res += "    template<typename R>\n";
     res += "    class " + type + " : " + basename + "<R> {" +"\n";
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
            if (field_type == basename){
                // need to add template type here 
                field_type += "<R>";
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

     // construct func
     tolower_inplace(type);
     tolower_inplace(basename);
     func += "              visitor.visit_"+type+"_"+basename+"(this);\n";
     func += "         }\n";
     return res + constructor + func + "    };\n";     
}

void Interpreter::define_abstract_syntax_tree(std::string basename, std::vector<std::string> types, std::string output_dir){
    fs::path output_file = fs::path(output_dir) / (basename + ".cpp") ;
    std::ofstream of {output_file};
    if (!of.is_open()){
        spdlog::error("Failed to open file at {} for output", output_file.string());
    }
    of << "#include <token_type.h>\n\n";
    of << "namespace Interpreter {\n";
    for (auto& type: types){
        size_t delim_pos = type.find(":");
        of << "    template<typename R>\n";
        of << "    class " + strip(type.substr(0, delim_pos)) + ";\n";
    }
    of << "    template<typename R>\n";
    of << "    class Visitor;\n";
    of << "    template<typename R>\n";
    of << "    class " << basename << " {\n";
    of << "         virtual R accept(Visitor<R> visitor);\n";
    of << "    };\n";
    of << define_visitor(basename, types);
    of << "\n";
    for (auto &type: types){
        size_t delim_pos = type.find(':');
        of << define_type(basename, type.substr(0, delim_pos), type.substr(delim_pos + 1));
    }
    of << "}";
    of.close();
}
