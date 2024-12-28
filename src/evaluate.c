#include "evaluate.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
        case UNARY:{
            // Handle unary operations like negation
            if (expr->as.unary.unary_op.type == MINUS) {
                EvalResult inner_result = evaluate_expr(expr->as.unary.expression);
                if (inner_result.is_number) {
                    result.is_number = true;
                    result.number_value = -inner_result.number_value;  // Apply negation
                }
            }
            else if (expr->as.unary.unary_op.type == BANG) {
                EvalResult inner_result = evaluate_expr(expr->as.unary.expression);
                if (inner_result.is_boolean) {
                    result.is_boolean = true;
                    result.boolean_value = !inner_result.boolean_value;  // Apply negation
                }
                else if (inner_result.is_nil) {
                    result.is_boolean = true;
                    result.boolean_value = true;  // Negate nil, which is falsy, so result is true
                }
                else if (inner_result.is_number) {
                    // For numbers: non-zero is truthy (negates to false), zero is falsy (negates to true)
                    result.is_boolean = true;
                    result.boolean_value = (inner_result.number_value == 0) ? true : false; // Treat non-zero as true, zero as false
                }
            }
            break;
        }
        case BINARY:{
            EvalResult left_result = evaluate_expr(expr->as.binary.left);
            EvalResult right_result = evaluate_expr(expr->as.binary.right);

            // Perform the binary operation based on the operator type
            if (expr->as.binary.binary_op.type == STAR) {  // Multiplication
                if (left_result.is_number && right_result.is_number) {
                    result.is_number = true;
                    result.number_value = left_result.number_value * right_result.number_value;
                }
            } else if (expr->as.binary.binary_op.type == SLASH) {  // Division
                if (left_result.is_number && right_result.is_number) {
                    result.is_number = true;
                    result.number_value = left_result.number_value / right_result.number_value;
                }
            }
            else if (expr->as.binary.binary_op.type == PLUS) {  // Addition
                if (left_result.is_number && right_result.is_number) {
                    result.is_number = true;
                    result.number_value = left_result.number_value + right_result.number_value;
                } 
                else if (left_result.is_string && right_result.is_string) {  // String concatenation
                    result.is_string = true;
                    result.string_value = malloc(strlen(left_result.string_value) + strlen(right_result.string_value) + 1);
                    strcpy(result.string_value, left_result.string_value);
                    strcat(result.string_value, right_result.string_value);
                }
            } else if (expr->as.binary.binary_op.type == MINUS) {  // Subtraction
                if (left_result.is_number && right_result.is_number) {
                    result.is_number = true;
                    result.number_value = left_result.number_value - right_result.number_value;
                }
            }else if (expr->as.binary.binary_op.type == GREATER) {  
                if (left_result.is_number && right_result.is_number) {
                    result.is_boolean = true;
                    result.boolean_value = left_result.number_value > right_result.number_value;
                }
            }
            else if (expr->as.binary.binary_op.type == LESS) {  
                if (left_result.is_number && right_result.is_number) {
                    result.is_boolean = true;
                    result.boolean_value = left_result.number_value < right_result.number_value;
                }
            }
            else if (expr->as.binary.binary_op.type == GREATER_EQUAL) {  
                if (left_result.is_number && right_result.is_number) {
                    result.is_boolean = true;
                    result.boolean_value = left_result.number_value >= right_result.number_value;
                }
            }
            else if (expr->as.binary.binary_op.type == LESS_EQUAL) {  
                if (left_result.is_number && right_result.is_number) {
                    result.is_boolean = true;
                    result.boolean_value = left_result.number_value <= right_result.number_value;
                }
            }
            else if (expr->as.binary.binary_op.type == EQUAL_EQUAL) {  
                if (left_result.is_string && right_result.is_string) {
                    result.is_boolean = true;
                    if(strcmp(left_result.string_value,right_result.string_value)==0){
                        result.boolean_value = true;
                    }
                    else{
                        result.boolean_value = false;
                    } 
                }
                else if (left_result.is_number && right_result.is_number) {
                    result.is_boolean = true;
                    result.boolean_value = left_result.number_value == right_result.number_value;
                }
                else{
                    result.is_boolean = true;
                    result.boolean_value = false;
                }
                

            }
            else if (expr->as.binary.binary_op.type == BANG_EQUAL) {  
                if (left_result.is_number && right_result.is_number) {
                    result.is_boolean = true;
                    result.boolean_value = left_result.number_value != right_result.number_value;
                }
                else if (left_result.is_string && right_result.is_string) {
                    result.is_boolean = true;
                    if(strcmp(left_result.string_value,right_result.string_value)!=0){
                        result.boolean_value = true;
                    }
                    else{
                        result.boolean_value = false;
                    } 
                }
            }
            break;
        }  
        
        
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
