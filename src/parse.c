#include "parse.h"
#include <stdlib.h>
#include <stdio.h>

// Parser helpers
Token* peek(Parser* parser) {
    return parser->current < parser->tokens->size ? parser->tokens->array[parser->current] : NULL;
}

Token* advance(Parser* parser) {
    if (parser->current < parser->tokens->size) {
        parser->current++;
    }
    return peek(parser);
}

bool match(Parser* parser, enum TokenType type) {
    if (peek(parser) && peek(parser)->type == type) {
        advance(parser);
        return true;
    }
    return false;
}


Expr* parse_primary(Parser* parser) {
    if (match(parser, TRUE) || match(parser, FALSE) || match(parser, NIL) || match(parser, NUMBER) || match(parser, STRING)) {
        return create_literal_expr(*parser->tokens->array[parser->current - 1]);
    }

    if (match(parser, LEFT_PAREN)) {
        Expr* expr = parse_expression(parser);
        if (!match(parser, RIGHT_PAREN)) {
            fprintf(stderr, "Error: Expected ')'.\n");
            exit(1);
        }
        return create_grouping_expr(expr);
    }

    fprintf(stderr, "Error: Unexpected token.\n");
    exit(1);
}


Expr* parse_expression(Parser* parser) {
    return parse_term(parser);
}

Expr* parse_term(Parser* parser) {
    Expr* expr = parse_primary(parser);
    while (match(parser, PLUS) || match(parser, MINUS)) {
        Token operator = *parser->tokens->array[parser->current - 1];
        Expr* right = parse_primary(parser);
        expr = create_binary_expr(operator,expr, right);
    }
    
    return expr;
}



// Parser creation
Parser* create_parser(TokenArray* tokens) {
    Parser* parser = malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->current = 0;
    return parser;
}

void free_parser(Parser* parser) {
    free(parser);
}



