#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"

typedef struct {
    TokenArray* tokens;
    size_t current;
} Parser;

Parser* create_parser(TokenArray* tokens);
Expr* parse_expression(Parser* parser);
Expr* parse_term(Parser* parser);
Expr* parse_primary(Parser* parser);
bool match(Parser* parser, enum TokenType type);
Token* advance(Parser* parser);
Token* peek(Parser* parser);
void free_parser(Parser* parser);

#endif
