#ifndef TOKENIZER
#define TOKENIZER


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


enum TokenType{
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    STAR,
    EQUAL,
    EQUAL_EQUAL,
    BANG,
    BANG_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    SLASH,
    STRING,
    NUMBER,
    IDENTIFIER,
    AND,
    CLASS,
    ELSE,
    FALSE,
    FOR,
    FUN,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,
    END_OF_FILE
};


typedef struct Token{
    enum TokenType type;
    char *lexeme; 
    void *literal; 
    int line;
    int length; 
} Token;


typedef struct {
    size_t capacity; 
    size_t size;
    Token **array;
} TokenArray;


typedef struct {
  const char *c;
  enum TokenType type;
} Keyword;

static Keyword keywords[] = {
    {"and", AND},     {"class", CLASS},   {"else", ELSE},
    {"false", FALSE}, {"for", FOR},       {"fun", FUN},
    {"if", IF},       {"nil", NIL},       {"or", OR},
    {"print", PRINT}, {"return", RETURN}, {"super", SUPER},
    {"this", THIS},   {"true", TRUE},     {"var", VAR},
    {"while", WHILE}
    };

Token *create_token(void);

TokenArray *create_token_array(void);

void append(TokenArray *array, Token *t);

const char* token_type_as_string(const enum TokenType type);

void print_token(const Token *t);

void print_token_array(const TokenArray *a);

int scan_tokens(TokenArray *a, char *source);

int isAlpha(char c);

enum TokenType lookup_keywords(const char *lexeme, int length);
#endif