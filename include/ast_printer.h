#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <expr.h>
#include <rd_parser.h>

namespace Interpreter{
    class AstPrinter: public Visitor<std::any> {
        private:
            std::string parenthesize(std::string name, std::vector<std::shared_ptr<Expr>> exprs); 
        public:
            std::any visit_binary_expr(const Binary& expr) override;
            std::any visit_grouping_expr(const Grouping& expr) override;
            std::any visit_literal_expr(const Literal& expr) override;
            std::any visit_unary_expr(const Unary& expr) override;
            std::string print(const Expr& expr);
    };
}

