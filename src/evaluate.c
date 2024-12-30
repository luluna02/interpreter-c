#include "evaluate.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



EvalResult evaluate_expr(Expr* expr) {
    switch (expr->type) {
        case LITERAL:
            return visit_literal(expr);
        case BINARY:
            return visit_binary(expr);
        case UNARY:
            return visit_unary(expr);
        case GROUPING:
            return visit_grouping(expr);
        default:
            fprintf(stderr, "Unexpected expression type\n");
            exit(65);
    }
}

EvalResult visit_literal(Expr* expr) {
    EvalResult result = {0};
    Token value = expr->as.literal.value;

    switch (value.type) {
        case TRUE:
            result.is_boolean = true;
            result.boolean_value = true;
            break;
        case FALSE:
            result.is_boolean = true;
            result.boolean_value = false;
            break;
        case NIL:
            result.is_nil = true;
            break;
        case NUMBER:
            result.is_number = true;
            result.number_value = *((double*)value.literal);
            break;
        case STRING:
            result.is_string = true;
            result.string_value = strdup((char*)value.literal);
            break;
        default:
            fprintf(stderr, "Unexpected literal type\n");
            exit(65);
    }

    return result;
}

EvalResult visit_grouping(Expr* expr) {
    return evaluate_expr(expr->as.grouping.expression);
}

EvalResult visit_unary(Expr* expr) {
    EvalResult result = {0};
    EvalResult inner_result = evaluate_expr(expr->as.unary.expression);

    switch (expr->as.unary.unary_op.type) {
        case MINUS:
            if (inner_result.is_number) {
                result.is_number = true;
                result.number_value = -inner_result.number_value;
            }
            else{
                runtime_error("Operand must be a number.", expr->as.unary.unary_op.line);
            }
            break;
        case BANG:
            result.is_boolean = true;
            if (inner_result.is_boolean) {
                result.boolean_value = !inner_result.boolean_value;
            } else if (inner_result.is_nil) {
                result.boolean_value = true;
            } else if (inner_result.is_number) {
                result.boolean_value = (inner_result.number_value == 0);
            }
            break;
        default:
            fprintf(stderr, "Unexpected unary operator\n");
            exit(65);
    }

    return result;
}

EvalResult visit_binary(Expr* expr) {
    EvalResult result = {0};
    EvalResult left_result = evaluate_expr(expr->as.binary.left);
    EvalResult right_result = evaluate_expr(expr->as.binary.right);

    switch (expr->as.binary.binary_op.type) {
        case STAR:
            if (left_result.is_number && right_result.is_number) {
                result.is_number = true;
                result.number_value = left_result.number_value * right_result.number_value;
            }
            else{
                runtime_error("Operands must be numbers.", expr->as.binary.binary_op.line);
            }
            break;
        case SLASH:
            if (left_result.is_number && right_result.is_number) {
                result.is_number = true;
                if (right_result.number_value == 0) {
                    runtime_error("Division by zero is not allowed.", expr->as.binary.binary_op.line);
                }else{
                    result.number_value = left_result.number_value / right_result.number_value;
                }
            }
            else{
                runtime_error("Operands must be numbers.", expr->as.binary.binary_op.line);
            }
            break;
        case PLUS:
            if (left_result.is_number && right_result.is_number) {
                result.is_number = true;
                result.number_value = left_result.number_value + right_result.number_value;
            } else if (left_result.is_string && right_result.is_string) {
                result.is_string = true;
                result.string_value = malloc(strlen(left_result.string_value) + strlen(right_result.string_value) + 1);
                strcpy(result.string_value, left_result.string_value);
                strcat(result.string_value, right_result.string_value);
            }
            break;
        case MINUS:
            if (left_result.is_number && right_result.is_number) {
                result.is_number = true;
                result.number_value = left_result.number_value - right_result.number_value;
            }
            break;
        case GREATER:
            if (left_result.is_number && right_result.is_number) {
                result.is_boolean = true;
                result.boolean_value = left_result.number_value > right_result.number_value;
            }
            break;
        case LESS:
            if (left_result.is_number && right_result.is_number) {
                result.is_boolean = true;
                result.boolean_value = left_result.number_value < right_result.number_value;
            }
            break;
        case GREATER_EQUAL:
            if (left_result.is_number && right_result.is_number) {
                result.is_boolean = true;
                result.boolean_value = left_result.number_value >= right_result.number_value;
            }
            break;
        case LESS_EQUAL:
            if (left_result.is_number && right_result.is_number) {
                result.is_boolean = true;
                result.boolean_value = left_result.number_value <= right_result.number_value;
            }
            break;
        case EQUAL_EQUAL:
            result.is_boolean = true;
            if (left_result.is_string && right_result.is_string) {
                result.boolean_value = strcmp(left_result.string_value, right_result.string_value) == 0;
            } else if (left_result.is_number && right_result.is_number) {
                result.boolean_value = left_result.number_value == right_result.number_value;
            } else {
                result.boolean_value = false;
            }
            break;
        case BANG_EQUAL:
            result.is_boolean = true;
            if (left_result.is_string && right_result.is_string) {
                result.boolean_value = strcmp(left_result.string_value, right_result.string_value) != 0;
            } else if (left_result.is_number && right_result.is_number) {
                result.boolean_value = left_result.number_value != right_result.number_value;
            }
            break;
        default:
            fprintf(stderr, "Unexpected binary operator\n");
            exit(65);
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

void free_eval_result(EvalResult *result) {
    if (result->is_string) {
        free(result->string_value);  // Free the string if it was allocated
    }
    // No need to free the number or boolean, as they are primitive types
}


void runtime_error(const char *message, int line) {
  fprintf(stderr, "Runtime error: %s\n[line %d]\n", message, line);
  exit(70);
}