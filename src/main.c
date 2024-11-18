#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "parse.h"
#include "ast.h"


char *read_file_contents(const char *filename);


int main(int argc, char *argv[]) {
    // Disable output buffering
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    

    if (argc < 3) {
        fprintf(stderr, "Usage: ./your_program tokenize <filename>\n");
        return 1;
    }
    int exit_code = 0;

    const char *command = argv[1];


    if (strcmp(command, "tokenize") == 0) {

        fprintf(stderr, "Logs from your program will appear here!\n");
        
        char *file_contents = read_file_contents(argv[2]);

        TokenArray *tokens = create_token_array();
        exit_code = scan_tokens(tokens, file_contents);
        print_token_array(tokens);

        free(file_contents);

    }
    else if (strcmp(command, "parse") == 0) {
        fprintf(stderr, "Parsing input file...\n");

        char *file_contents = read_file_contents(argv[2]);
        if (!file_contents) return 1;
        TokenArray *tokens = create_token_array();
        exit_code = scan_tokens(tokens, file_contents);

        if (exit_code != 0) {
            fprintf(stderr, "Error during tokenization\n");
            free(file_contents);
            return exit_code;
        }

        Parser *parser = create_parser(tokens);
        Expr *ast = parse_expression(parser);
        if (parser->had_error== true) {
            fprintf(stderr, "Error parsing AST\n");
            exit_code = 65;
        } else {
            print_ast(ast);  // Print the AST
        }
        free_expr(ast);
        free_parser(parser);
        free(file_contents);
    } else {
        fprintf(stderr, "Unknown command: %s\n", command);
        return 1;
    }

    
    return exit_code;
    
}

char *read_file_contents(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error reading file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *file_contents = malloc(file_size + 1);
    if (file_contents == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(file_contents, 1, file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "Error reading file contents\n");
        free(file_contents);
        fclose(file);
        return NULL;
    }

    file_contents[file_size] = '\0';
    fclose(file);

    return file_contents;
}


