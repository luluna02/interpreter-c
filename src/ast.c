#include "ast.h"
#include <stdlib.h>

Expr* create_binary_expr(Token binary_op, Expr* left, Expr* right) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = BINARY;
    expr->as.binary.left = left;
    expr->as.binary.binary_op = binary_op;
    expr->as.binary.right = right;
    return expr;
}

Expr* create_literal_expr(Token value) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = LITERAL;
    expr->as.literal.value = value;
    return expr;
}

Expr* create_grouping_expr(Expr* expression) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = GROUPING;
    expr->as.grouping.expression = expression;
    return expr;
}

Expr* create_unary_expr(Token unary_op, Expr* expression) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = UNARY;
    expr->as.unary.unary_op = unary_op;
    expr->as.unary.expression = expression;
    return expr;
}


void free_expr(Expr* expr) {
    if (!expr) return;
    switch (expr->type) {
        case BINARY:
            free_expr(expr->as.binary.left);
            free_expr(expr->as.binary.right);
            break;
        case GROUPING:
            free_expr(expr->as.grouping.expression);
            break;
        case UNARY:
            free_expr(expr->as.unary.expression);
        case LITERAL:
            break;
    }
    free(expr);
}

void print_ast(Expr *expr) {
    if (expr->type == LITERAL) {
        if (expr->as.literal.value.literal != NULL) {
        if (expr->as.literal.value.type == STRING) {
            printf("%s", (char *)expr->as.literal.value.literal); // Add a space between lexeme and string literal
        }
        else if (expr->as.literal.value.type == NUMBER) {
            double value = *(double *)expr->as.literal.value.literal;
            if (value == (int)value) {
                printf("%.1f", value); // Format number with a space before it
            } else {
                printf("%g", value);
            }
        }
        }
        else{
            for(int i = 0; i < expr->as.literal.value.length; ++i)
        {
            printf("%c", expr->as.literal.value.lexeme[i]);
        }
        }
    } else if (expr->type == BINARY) {
        printf("(");
        for(int i = 0; i < expr->as.binary.binary_op.length; ++i)
        {
            printf("%c", expr->as.binary.binary_op.lexeme[i]);
            if(i == (expr->as.binary.binary_op.length - 1)){
                printf(" ");
            }
        }
        print_ast(expr->as.binary.left);
        printf(" ");
        print_ast(expr->as.binary.right);
        printf(")");

    }
    else if(expr->type == GROUPING){
        printf("(group ");
        print_ast(expr->as.grouping.expression); // Recursively print the inner expression
        printf(")");
    }
    else if(expr->type == UNARY){
        printf("(");
        for(int i = 0; i < expr->as.unary.unary_op.length; ++i)
        {
            printf("%c ", expr->as.unary.unary_op.lexeme[i]);
        }
        print_ast(expr->as.unary.expression); // Recursively print the inner expression
        printf(")");
    }
}



