#ifndef TOKENIZER
#define TOKENIZER

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    END_OF_FILE,
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


Token *create_token(void);

TokenArray *create_token_array(void);

void append(TokenArray *array, Token *t);

const char* token_type_as_string(const enum TokenType type);

void print_token(const Token *t);

void print_token_array(const TokenArray *a);

int scan_tokens(TokenArray *a, char *source);

int isAlpha(char c);

#endif