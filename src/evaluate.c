#include "evaluate.h"
#include <stdlib.h>
#include <stdio.h>

EvalResult evaluate_expr(Expr* expr) {
    EvalResult result = {0};
    switch (expr->type) {
        case LITERAL: {
            Token value = expr->as.literal.value;
            if (value.type == TRUE) {
                result.is_boolean = true;
                result.boolean_value = true;
            } else if (value.type == FALSE) {
                result.is_boolean = true;
                result.boolean_value = false;
            } else if (value.type == NIL) {
                result.is_nil = true;
            }
            else if (value.type == NUMBER) {
                result.is_number = true;
                result.number_value = *((double*)value.literal);;
            } else if (value.type == STRING) {
                result.is_string = true;
                result.string_value = strdup((char*)value.literal); // Copy string
            }
            break;
        }
        case GROUPING: {
            return evaluate_expr(expr->as.grouping.expression);
        }
        case UNARY:
        case BINARY:
            // Implement unary and binary expression evaluation
            break;
    }

    return result;
}

void print_eval_result(EvalResult result){
    if (result.is_nil) {
        printf("nil\n");
    } else if (result.is_boolean) {
        printf("%s\n", result.boolean_value ? "true" : "false");
    } else if (result.is_number) {
        printf("%.10g\n", result.number_value);
    } else if (result.is_string) {
        printf("%s\n", result.string_value);
    } else {
        fprintf(stderr, "Unexpected evaluation result\n");
    }
}


