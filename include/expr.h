#include <token_type.h>
#include<any>

namespace Interpreter {
    class Binary;
    class Grouping;
    class Literal;
    class Unary;
    template<typename R>
    class Visitor;
    class Expr {
         public:
             virtual std::any accept(Visitor<std::any>& visitor) const = 0;
    };
    template<typename R>
        class Visitor{
            public:
                virtual R visit_binary_expr(const Binary& expr) = 0;
                virtual R visit_grouping_expr(const Grouping& expr) = 0;
                virtual R visit_literal_expr(const Literal& expr) = 0;
                virtual R visit_unary_expr(const Unary& expr) = 0;
        };

    class Binary : public Expr {
        public:
         const std::shared_ptr<Expr>  left;
         const Token  op;
         const std::shared_ptr<Expr>  right;
         Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right): left(left), op(op), right(right) {}
         std::any accept(Visitor<std::any>& visitor) const {
              return visitor.visit_binary_expr(*this);
         }
    };
    class Grouping : public Expr {
        public:
         const std::shared_ptr<Expr>  expression;
         Grouping(std::shared_ptr<Expr> expression): expression(expression) {}
         std::any accept(Visitor<std::any>& visitor) const {
              return visitor.visit_grouping_expr(*this);
         }
    };
    class Literal : public Expr {
        public:
         const std::any  value;
         Literal(std::any value): value(value) {}
         std::any accept(Visitor<std::any>& visitor) const {
              return visitor.visit_literal_expr(*this);
         }
    };
    class Unary : public Expr {
        public:
         const Token  op;
         const std::shared_ptr<Expr>  right;
         Unary(Token op, std::shared_ptr<Expr> right): op(op), right(right) {}
         std::any accept(Visitor<std::any>& visitor) const {
              return visitor.visit_unary_expr(*this);
         }
    };
}