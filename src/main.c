#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *read_file_contents(const char *filename);

int main(int argc, char *argv[]) {
    // Disable output buffering
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    if (argc < 3) {
        fprintf(stderr, "Usage: ./your_program tokenize <filename>\n");
        return 1;
    }


    const char *command = argv[1];
    int EXIT_CODE = EXIT_SUCCESS;

    if (strcmp(command, "tokenize") == 0) {

        fprintf(stderr, "Logs from your program will appear here!\n");
        
        char *file_contents = read_file_contents(argv[2]);


        if (strlen(file_contents) > 0) {
            for (int i = 0; i < strlen(file_contents); i++) { 
                if (file_contents[i] == '\0') {
                break; 
                }
                switch (file_contents[i]) {
                    case '(':
                        printf("LEFT_PAREN ( null\n");
                        break;
                    case ')':
                        printf("RIGHT_PAREN ) null\n");
                        break;
                    case '{':
                        printf("LEFT_BRACE { null\n");
                        break;
                    case '}':
                        printf("RIGHT_BRACE } null\n");
                        break;
                    case ',':
                        printf("COMMA , null\n");
                        break;
                    case '.':
                        printf("DOT . null\n");
                        break;
                    case '-':
                        printf("MINUS - null\n");
                        break;
                    case '+':
                        printf("PLUS + null\n");
                        break;
                    case ';':
                        printf("SEMICOLON ; null\n");
                        break;
                    case '*':
                        printf("STAR * null\n");
                        break;
                    case '=':
                        // check if anoother = exists?
                        if(file_contents[i+1] == '='){
                            printf("EQUAL_EQUAL == null\n");
                            i++;
                        }
                        else{
                            printf("EQUAL = null\n");
                        }
                        break;
                    case '!':
                        // check if anoother = exists?
                        if(file_contents[i+1] == '='){
                            printf("BANG_EQUAL != null\n");
                            i++;
                        }
                        else{
                            printf("BANG ! null\n");
                        }
                        break;
                    case '<':
                        // check if anoother = exists?
                        if(file_contents[i+1] == '='){
                            printf("LESS_EQUAL <= null\n");
                            i++;
                        }
                        else{
                            printf("LESS < null\n");
                        }
                        break;
                    case '>':
                        // check if anoother = exists?
                        if(file_contents[i+1] == '='){
                            printf("GREATER_EQUAL >= null\n");
                            i++;
                        }
                        else{
                            printf("GREATER > null\n");
                        }
                        break;
                    case '/':
                        // check if anoother = exists?
                        if(file_contents[i+1] == '/'){
                            int temp = i;
                            while (file_contents[temp] != '\n' && file_contents[temp] != '\0'){
                                temp++;
                            }
                            i = temp+1;
                        }
                        else{
                            printf("SLASH / null\n");
                        }
                        break;
                    default:
                        fprintf(stderr,"[line %d] Error: Unexpected character: %c\n", 1, file_contents[i]);
                        EXIT_CODE = 65;
                        break;
                }
            }
        }

        printf("EOF  null\n");
        free(file_contents);

    } else {
        fprintf(stderr, "Unknown command: %s\n", command);
        return 1;
    }
    return EXIT_CODE;
    
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
