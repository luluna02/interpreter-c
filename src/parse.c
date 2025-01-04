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


void parse_program(Parser* parser, StmtArray* statements) {
    while (!is_at_end(parser)) {
        Stmt* stmt = parse_statement(parser);
        if (stmt != NULL) {
            append_statement(statements, stmt);
        } else {
            synchronize(parser); // Recover from errors and continue parsing.
        }
    }
}


Stmt* parse_statement(Parser* parser) {
    if (match(parser, PRINT)) {
        Expr* value = parse_expression(parser);
        if (!match(parser, SEMICOLON)) {
            report_error(parser, peek(parser), "Expected ';' after value.");
            return NULL;
        }
        return create_print_stmt(value);
    }
    return parse_expression_statement(parser);
}

Stmt* parse_expression_statement(Parser* parser) {
    Expr* expr = parse_expression(parser);
    if (!match(parser, SEMICOLON)) {
        report_error(parser, peek(parser), "Expected ';' after expression.");
        return NULL;
    }
    return create_expression_stmt(expr);
}



Expr* parse_expression(Parser* parser) {
    return parse_equality(parser);
}

Expr* parse_equality(Parser* parser) {
    Expr* expr = parse_comparison(parser);
    while (match(parser, EQUAL_EQUAL) || match(parser, BANG_EQUAL)) {
        Token operator = *parser->tokens->array[parser->current - 1];
        Expr* right = parse_comparison(parser);
        expr = create_binary_expr(operator,expr, right);
    }
    return expr;
}

Expr* parse_comparison(Parser* parser) {
    Expr* expr = parse_term(parser);
    while (match(parser, LESS) || match(parser, LESS_EQUAL) || match(parser, GREATER) || match(parser, GREATER_EQUAL)) {
        Token operator = *parser->tokens->array[parser->current - 1];
        Expr* right = parse_term(parser);
        expr = create_binary_expr(operator,expr, right);
    }
    return expr;
}

Expr* parse_term(Parser* parser) {
    Expr* expr = parse_factor(parser);
    while (match(parser, PLUS) || match(parser, MINUS)) {
        Token operator = *parser->tokens->array[parser->current - 1];
        Expr* right = parse_factor(parser);
        expr = create_binary_expr(operator,expr, right);
    }
    return expr;
}

Expr* parse_factor(Parser* parser) {
    Expr* expr = parse_unary(parser);
    while (match(parser, SLASH) || match(parser, STAR)) {
        Token operator = *parser->tokens->array[parser->current - 1];
        Expr* right = parse_unary(parser);
        expr = create_binary_expr(operator,expr, right);
    }
    return expr;
}

Expr* parse_unary(Parser* parser) {
    if (match(parser, BANG) || match(parser, MINUS)) {
        Token operator = *parser->tokens->array[parser->current - 1];
        Expr* right = parse_unary(parser);
        return create_unary_expr(operator, right);
    }
    return parse_primary(parser);
}

Expr* parse_primary(Parser* parser) {
    if (match(parser, TRUE) || match(parser, FALSE) || match(parser, NIL) || match(parser, NUMBER) || match(parser, STRING)) {
        return create_literal_expr(*parser->tokens->array[parser->current - 1]);
    }

    if (match(parser, LEFT_PAREN)) {
        Expr* expr = parse_expression(parser);
        if (!match(parser, RIGHT_PAREN)) {
            report_error(parser, peek(parser), "Expected ')'.");
            return NULL;
        }
        return create_grouping_expr(expr);
    }

    report_error(parser, peek(parser), "Expected an expression.");
    return NULL;
}



// Parser creation
Parser* create_parser(TokenArray* tokens) {
    Parser* parser = malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->current = 0;
    parser->had_error = false;
    return parser;
}

void free_parser(Parser* parser) {
    free(parser);
}



// 

bool is_at_end(Parser* parser) {
    Token* token = peek(parser);
    return token && token->type == END_OF_FILE; // Adjust EOF_TOKEN to match your EOF type definition
}

void report_error(Parser* parser, Token* token, const char* message) {
    if (token->type == END_OF_FILE) {
        fprintf(stderr, "[line %d] Error at end: %s\n", token->line, message);
    } else {
        fprintf(stderr, "[line %d] Error at '%s': %s\n", token->line, token->lexeme, message);
    }
    parser->had_error = true;
}



void synchronize(Parser* parser) {
    advance(parser);

    while (!is_at_end(parser)) {
        if (previous(parser)->type == SEMICOLON) return;

        switch (peek(parser)->type) {
            case PRINT:  
            case END_OF_FILE:   
                return;

            default:
                break;
        }
        advance(parser);
    }
}

Token* previous(Parser* parser) {
    if (parser->current > 0) {
        return parser->tokens->array[parser->current - 1];  // Access the token from the array field.
    }
    return NULL;  
}
