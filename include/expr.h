#include <token_type.h>

namespace Interpreter {
    class Expr {};
    class Binary : Expr {
        public:
         const Expr  left;
         const Token  op;
         const Expr  right;
         Binary(Expr left, Token op, Expr right): left(left), op(op), right(right) {}
    };
    class Grouping : Expr {
        public:
         const Expr  expression;
         Grouping(Expr expression): expression(expression) {}
    };
    class Literal : Expr {
        public:
         const std::any  value;
         Literal(std::any value): value(value) {}
    };
    class Unary : Expr {
        public:
         const Token  op;
         const Expr  right;
         Unary(Token op, Expr right): op(op), right(right) {}
    };
}