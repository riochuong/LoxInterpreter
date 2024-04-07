#include <string>
#include <vector>
#include <token_type.h>

namespace Interpreter{
    std::string define_type(std::string basename, std::string type, std::string field_list);
    void define_abstract_syntax_tree(std::string basename, std::vector<std::string> types, std::string output_dir);
    std::string define_visitor(std::string basename, std::vector<std::string> types);
}
