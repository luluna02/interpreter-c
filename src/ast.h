#ifndef AST_H
#define AST_H

#include "token.h"

typedef struct Expr Expr;

typedef struct {
    Token binary_op;
    Expr* left;
    Expr* right;
} BinaryExpr;

typedef struct {
    Token value;
} LiteralExpr;

typedef struct {
    Expr* expression;
} GroupingExpr;

typedef struct {
    Expr* expression;
    Token unary_op;
} UnaryExpr;

struct Expr {
    enum { BINARY, LITERAL, GROUPING, UNARY } type;
    union {
        BinaryExpr binary;
        LiteralExpr literal;
        GroupingExpr grouping;
        UnaryExpr unary;
    } as;
};


Expr* create_binary_expr(Token binary_op, Expr* left, Expr* right);
Expr* create_literal_expr(Token value);
Expr* create_grouping_expr(Expr* expression);
Expr* create_unary_expr(Token unary_op, Expr* expression);
void print_ast(Expr *expr);
void free_expr(Expr* expr);
#endif
