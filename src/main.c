#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char *read_file_contents(const char *filename);

int main(int argc, char *argv[]) {
    // Disable output buffering
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    int temp = 0;

    if (argc < 3) {
        fprintf(stderr, "Usage: ./your_program tokenize <filename>\n");
        return 1;
    }
    int current_line = 1;

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
                        temp = i;
                        if(file_contents[i+1] == '/'){
                            
                            while (file_contents[temp] != '\n' && file_contents[temp] != '\0'){
                                temp++;
                            }
                            i = temp;
                            if (file_contents[temp] == '\n') {
                            ++current_line;
                        }
                        }
                        else{
                            printf("SLASH / null\n");
                        }
                        break;

                    case '"':{  
                        int start = i;
                        i++;  // Move past the opening quote
                        int str_start = i;

                        // Scan until closing quote or end of file
                        while (file_contents[i] != '"' && file_contents[i] != '\0') {
                            if (file_contents[i] == '\n') ++current_line;
                            i++;
                        }

                        if (file_contents[i] == '\0') {
                            // Unterminated string
                            fprintf(stderr, "[line %d] Error: Unterminated string.\n", current_line);
                            EXIT_CODE = 65;
                        } else {
                            // Extract the string content
                            int len = i - str_start;
                            char *literal = (char *)malloc(len + 1);
                            strncpy(literal, &file_contents[str_start], len);
                            literal[len] = '\0';

                            // Print STRING token with lexeme and literal
                            printf("STRING %.*s %s\n", len + 2, &file_contents[start], literal);

                            free(literal);
                        }
                        break;
                    }
                    case '0' ... '9': {  // Check if the current character is a digit.
                        int start = i;

                        // Parse the integer part.
                        while (i < strlen(file_contents) && isdigit(file_contents[i])) {
                            i++;
                        }

                        // Flag for detecting if it's a floating-point number.
                        int is_float = 0;

                        // Check for a decimal point followed by a digit (for floating-point numbers).
                        if (file_contents[i] == '.' && isdigit(file_contents[i + 1])) {
                            is_float = 1;  // Mark as a float
                            i++; // Include the dot.
                            while (i < strlen(file_contents) && isdigit(file_contents[i])) {
                                i++; // Parse the fractional part.
                            }
                        }

                        // Calculate the length of the lexeme.
                        int len = i - start;

                        // Extract the lexeme.
                        char lexeme[len + 1];
                        strncpy(lexeme, &file_contents[start], len);
                        lexeme[len] = '\0'; // Null-terminate the lexeme string.

                        // Prepare the literal value
                        char literal[50];
                        if (is_float) {
                            // Convert lexeme to double and format it to one decimal place
                            double value = atof(lexeme);
                            snprintf(literal, sizeof(literal), "%.1f", value);
                        } else {
                            // For integers, append ".0" to match the expected format
                            snprintf(literal, sizeof(literal), "%s.0", lexeme);
                        }

                        // Print the token with lexeme and formatted literal.
                        printf("NUMBER %s %.2f\n", lexeme, literal);

                        i--; // Adjust the loop counter after processing.
                        break;
                    }

                           

                        

                    case'$':
                    case'#':
                    case'@':
                    case'%':
                        fprintf(stderr,"[line %d] Error: Unexpected character: %c\n", current_line, file_contents[i]);
                        EXIT_CODE = 65;
                        break;
                    case ' ':
                    case '\r':
                    case '\t':
                        break;
                    case '\n':
                        ++current_line;
                    default:
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
