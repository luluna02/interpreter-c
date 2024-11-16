#include "token.h"
#include <string.h>
#include <ctype.h>

const size_t DEFAULT_ARRAY_CAPACITY = 4096;

Token *create_token(void)
{
    Token *t = (Token *)malloc(sizeof(Token));
    
    if (t == NULL)
    {
        printf("Token memory allocation failed...\n");
        exit(1);
    }

    return t;
}

TokenArray *create_token_array(void)
{
    TokenArray *a = (TokenArray *)malloc(sizeof(TokenArray));
    if (a == NULL)
    {
        printf("memory allocation failed...\n");
        exit(1);
    }
    a->array = (Token **)malloc(sizeof(Token *) * DEFAULT_ARRAY_CAPACITY);
    a->capacity = DEFAULT_ARRAY_CAPACITY;
    a->size = 0;

    return a;
}


void append(TokenArray *a, Token *t){
    if (a->size == a->capacity)
    {
        a->capacity *= 2;
        a->array = realloc(a->array, a->capacity * sizeof(Token));
    }
    a->array[a->size] = t;
    a->size++;
    
}


const char* token_type_as_string(const enum TokenType type){
    switch (type)
    {
    case LEFT_PAREN:
        return "LEFT_PAREN";
    case RIGHT_PAREN:
        return "RIGHT_PAREN";
    case LEFT_BRACE:
        return "LEFT_BRACE";
    case RIGHT_BRACE:
        return "RIGHT_BRACE";
    case COMMA:
        return "COMMA";
    case DOT:
        return "DOT";
    case MINUS:
        return "MINUS";
    case PLUS:
        return "PLUS";
    case SEMICOLON:
        return "SEMICOLON";
    case STAR:
        return "STAR";
    case EQUAL:
        return "EQUAL";
    case EQUAL_EQUAL:
        return "EQUAL_EQUAL";
    case BANG:
        return "BANG";
    case BANG_EQUAL:
        return "BANG_EQUAL";
    case LESS:
        return "LESS";
    case LESS_EQUAL:
        return "LESS_EQUAL";
    case GREATER:
        return "GREATER";
    case GREATER_EQUAL:
        return "GREATER_EQUAL";
    case SLASH:
        return "SLASH";
    case STRING:
        return "STRING";
    case NUMBER:
        return "NUMBER";
    case IDENTIFIER:
        return "IDENTIFIER";
    case END_OF_FILE:
        return "EOF";
    case AND:             
        return "AND";
    case CLASS:           
        return "CLASS";
    case ELSE:            
        return "ELSE";
    case FALSE:           
        return "FALSE";
    case FUN:             
        return "FUN";
    case FOR:             
        return "FOR";
    case IF:              
        return "IF";
    case NIL:             
        return "NIL";
    case OR:              
        return "OR";
    case PRINT:           
        return "PRINT";
    case RETURN:          
        return "RETURN";
    case SUPER:           
        return "SUPER";
    case THIS:            
        return "THIS";
    case TRUE:          
        return "TRUE";
    case VAR:             
        return "VAR";
    case WHILE:           
        return "WHILE";
    default:
        break;
    }
}

void print_token(const Token *t)
{
    // Print token type
    printf("%s ", token_type_as_string(t->type));

    // Print lexeme
    for(int i = 0; i < t->length; ++i)
    {
        printf("%c", t->lexeme[i]);
    }
    if (t->literal != NULL) {
        if (t->type == STRING) {
            printf(" %s\n", (char *)t->literal); // Add a space between lexeme and string literal
        }
        else if (t->type == NUMBER) {
            double value = *(double *)t->literal;
            if (value == (int)value) {
                printf(" %.1f\n", value); // Format number with a space before it
            } else {
                printf(" %g\n", value);
            }
        }
    }
    else {
        printf(" null\n"); // Print null without an extra space
    }
}

void print_token_array(const TokenArray *a)
{
    for(int i = 0; i < a->size; ++i)
    {
        print_token(a->array[i]);
    }
}


int scan_tokens(TokenArray *a, char *file_contents){
    int current_line = 1;
    int exit_code = 0;

    while(*file_contents){
        Token *t = create_token();
        t->length = 1;
        t->lexeme = file_contents;
        t->line = current_line;
        t->literal = NULL;

        char *temp = file_contents;
        switch(*file_contents){
            case '(':
            t->type = LEFT_PAREN;
            append(a, t);
            break;
            case ')':
            t->type = RIGHT_PAREN;
            append(a, t);
            break;
        case '{':
            t->type = LEFT_BRACE;
            append(a, t);
            break;
        case '}':
            t->type = RIGHT_BRACE;
            append(a, t);
            break;
        case ',':
            t->type = COMMA;
            append(a, t);
            break;
        case '.':
            t->type =  DOT;
            append(a, t);
            break;
        case '-':
            t->type = MINUS;
            append(a, t);
            break;
        case '+':
            t->type = PLUS;
            append(a, t);
            break;
        case ';':
            t->type = SEMICOLON;
            append(a, t);
            break;
        case '*':
            t->type = STAR;
            append(a, t);
            break;
        case '=':
            ++temp;
            if (*temp == '=')
            {
                t->type = EQUAL_EQUAL;
                t->length = 2;
                ++file_contents;
                append(a, t);
            }
            else
            {
                t->type = EQUAL;
                append(a, t);
            }
            break;
        case '!':
            ++temp;
            if (*temp == '=')
            {
                t->type = BANG_EQUAL;
                t->length = 2;
                ++file_contents;  
                append(a, t);
            }
            else
            {
                t->type = BANG;
                append(a, t);
            }
            break;
        case '<':
            ++temp;
            if (*temp == '=')
            {
                t->type = LESS_EQUAL;
                t->length = 2;
                ++file_contents;
                append(a, t);
            }
            else
            {
                t->type = LESS;
                append(a, t);
            }
            break;
        case '>':
            ++temp;
            if (*temp == '=')
            {
                t->type = GREATER_EQUAL;
                t->length = 2;
                ++file_contents;
                append(a, t);
            }
            else
            {
                t->type = GREATER;
                append(a, t);
            }
            break;
        case '/':
            ++temp;
            if (*temp == '/')
            {
                while(*temp != '\n' && *temp != '\0')
                {
                    ++temp;
                    ++file_contents;
                }
            }
            else
            {
                t->type = SLASH;
                append(a, t);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            ++current_line;
            break;
        case '"':
            ++temp;
             while(*temp != '"' && *temp != '\0'){
                if(*temp == '\n')
                {
                    ++current_line;
                }
                ++temp;
            }
            if (*temp == '\0')
            {
                fprintf(stderr, "[line %d] Error: Unterminated string.\n", current_line);
                --temp;
                exit_code = 65;
            }
            if (*temp == '"')
            {
                t->type = STRING;
                int l = (temp - file_contents);
                t->length = l+1;
                t->literal = (char *)malloc((l) * sizeof(char));
                strncpy((char *)t->literal, file_contents + 1, l - 1);
                ((char *)t->literal)[l - 1] = '\0';
                append(a, t);
            }
            file_contents = temp;
            break;

        case '0' ... '9':{

                int is_float =0;
                ++temp;
                while(isdigit(*temp) && *temp != '\0'){
                    temp++;
                }
                if (*temp == '.') {
                    is_float = 1;
                    temp++;
                    if (!isdigit(*temp)) {
                        fprintf(stderr, "[line %d] Error: Unterminated fraction.\n", current_line); 
                        while (*temp != '\n' && *temp != '\0') {
                            temp++;
                        }
                        file_contents = temp;
                        break;
                    } else {
                        while (isdigit(*temp)) {
                            temp++;
                        }
                    }
                }
                 


                t->type = NUMBER;
                int l = (temp - file_contents);

                char *string = (char *)malloc((l + 1) * sizeof(char));
                if (!string) {
                    fprintf(stderr, "Memory allocation failed for numeric string.\n");
                    exit(1);
                }
                strncpy(string, file_contents, l);
                string[l] = '\0';
                double *value = (double *)malloc(sizeof(double));
                if (!value) {
                    fprintf(stderr, "Memory allocation failed for numeric literal.\n");
                    exit(1);
                }

                *value = strtod(string, NULL);
                free(string);
                t->literal = value;

                t->length = l;
                append(a,t);
                file_contents = temp - 1;
                break;
            }
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '_':{ 
            temp++;
            while(isAlpha(*temp) || isdigit(*temp) && *temp != '\0' && *temp != '\n'){
                temp++;
            }
            int l = (temp - file_contents);
            t->length = l;
            t->type = lookup_keywords(file_contents, l);
            append(a, t);
            file_contents = temp-1;
            break;
        }   
        default:
            fprintf(stderr, "[line %d] Error: Unexpected character: %c\n", current_line, *file_contents);
            exit_code = 65;
            break;
        }
        file_contents++;
    }
    Token *t = create_token();
    t->type = END_OF_FILE;
    t->length = 0;
    t->lexeme = file_contents;
    t->line = current_line;
    t->literal = NULL;
    append(a, t);

    return exit_code;
}

int isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}


enum TokenType lookup_keywords(const char *lexeme, int length) {
    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strncmp(lexeme, keywords[i].c, length) == 0 && keywords[i].c[length] == '\0') {
            return keywords[i].type;
        }
    }
    return IDENTIFIER; 
}

