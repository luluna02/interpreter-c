#ifndef EVAL_H
#define EVAL_H

#include "ast.h"


// Define the result of evaluation
typedef struct {
    bool is_nil;         // Represents if the value is nil
    bool is_boolean;     // Represents if the value is a boolean
    bool boolean_value;  // Holds the boolean value
} EvalResult;

// Evaluate an expression
EvalResult evaluate_expr(Expr* expr);
void print_eval_result(EvalResult result);

#endif