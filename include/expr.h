#include <token_type.h>

namespace Interpreter {
    template<typename R>
    class Binary;
    template<typename R>
    class Grouping;
    template<typename R>
    class Literal;
    template<typename R>
    class Unary;
    template<typename R>
    class Visitor;
    template<typename R>
    class Expr {
         public:
             virtual R accept(Visitor<R>* visitor) = 0;
    };
    template<typename R>
        class Visitor{
            public:
                virtual R visit_binary_expr(Binary<R>* expr) = 0;
                virtual R visit_grouping_expr(Grouping<R>* expr) = 0;
                virtual R visit_literal_expr(Literal<R>* expr) = 0;
                virtual R visit_unary_expr(Unary<R>* expr) = 0;
        };

    template<typename R>
    class Binary : public Expr<R> {
        public:
         const std::shared_ptr<Expr<R>>  left;
         const Token  op;
         const std::shared_ptr<Expr<R>>  right;
         Binary(std::shared_ptr<Expr<R>> left, Token op, std::shared_ptr<Expr<R>> right): left(left), op(op), right(right) {}
         R accept(Visitor<R>* visitor){
              return visitor->visit_binary_expr(this);
         }
    };
    template<typename R>
    class Grouping : public Expr<R> {
        public:
         const std::shared_ptr<Expr<R>>  expression;
         Grouping(std::shared_ptr<Expr<R>> expression): expression(expression) {}
         R accept(Visitor<R>* visitor){
              return visitor->visit_grouping_expr(this);
         }
    };
    template<typename R>
    class Literal : public Expr<R> {
        public:
         const std::any  value;
         Literal(std::any value): value(value) {}
         R accept(Visitor<R>* visitor){
              return visitor->visit_literal_expr(this);
         }
    };
    template<typename R>
    class Unary : public Expr<R> {
        public:
         const Token  op;
         const std::shared_ptr<Expr<R>>  right;
         Unary(Token op, std::shared_ptr<Expr<R>> right): op(op), right(right) {}
         R accept(Visitor<R>* visitor){
              return visitor->visit_unary_expr(this);
         }
    };
}