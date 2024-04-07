#include <token_type.h>

namespace Interpreter {

    template<typename R>
    class Visitor {
        virtual R visit
    };

    template<typename R>
    class Expr {
        virtual R accept(Visitor<R> vistor);
    };

    template<typename R>
    class Binary : Expr<R> {
        public:
         const Expr<R>  left;
         const Token  op;
         const Expr<R>  right;
         Binary(Expr<R> left, Token op, Expr<R> right): left(left), op(op), right(right) {}
         R accept(Visitor<R> vistor){

         }
    };

    template<typename R>
    class Grouping : Expr {
        public:
         const Expr  expression;
         Grouping(Expr expression): expression(expression) {}
    };

    template<typename R>
    class Literal : Expr<R> {
        public:
         const std::any  value;
         Literal(std::any value): value(value) {}
    };

    template<typename R>
    class Unary : Expr<R> {
        public:
         const Token  op;
         const Expr<R>  right;
         Unary(Token op, Expr<R> right): op(op), right(right) {}
    };
}