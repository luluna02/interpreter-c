#ifndef EVAL_H
#define EVAL_H

#include "ast.h"


// Define the result of evaluation
typedef struct {
    bool is_nil;          // Represents if the value is nil
    bool is_boolean;      // Represents if the value is a boolean
    bool boolean_value;   // Holds the boolean value
    bool is_number;       // Represents if the value is a number
    double number_value;  // Holds the number value
    bool is_string;       // Represents if the value is a string
    char *string_value;   // Holds the string value (null-terminated)
} EvalResult;

// Evaluate an expression
EvalResult evaluate_expr(Expr* expr);
void print_eval_result(EvalResult result);
void free_eval_result(EvalResult *result);

#endif