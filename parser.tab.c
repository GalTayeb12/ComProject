/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

extern int yylineno;
extern int yylex();
void yyerror(const char* s);
extern char* yytext;

Node* root;


#define MAX_FUNCTIONS 100
FunctionInfo function_table[MAX_FUNCTIONS];
int function_count = 0;
int param_count_mismatch = 0;

#define MAX_VARS_PER_SCOPE 100
VarInfo var_info[MAX_VARS_PER_SCOPE];

#define MAX_DEPTH 100
char* var_names[MAX_VARS_PER_SCOPE];

int var_count = 0;
int duplicate_var_error = 0;
char return_type_stack[MAX_DEPTH][20];
int stack_top = -1;
char current_return_type[20] = "";
char return_type_to_push[20] = "";
int current_func_has_return = 0;

// דגלים חכמים
int param_error = 0;
int comma_error = 0;
int return_type_error = 0;
int missing_return_error = 0;
int proc_return_error = 0;
int param_count = 0;
int has_main = 0;
int multiple_main_error = 0;
int main_with_params_error = 0;
int duplicate_function_error = 0;
int undeclared_function_error = 0;
int undeclared_var_error = 0;
int param_type_mismatch = 0;
int string_return_error = 0;
int if_expr_type_error = 0;
int array_index_error = 0;
int string_index_error = 0;
int array_pointer_arithmetic_error = 0;
int non_string_array_access_error = 0;
int arith_type_error = 0;
int logical_type_error = 0;
int comparison_type_error = 0;
int equality_type_error = 0;
int length_type_error = 0;
int logical_op_type_error = 0;
int debug_mode = 1;
int invalid_address_error = 0;
int dereference_type_error = 0;
//int param_order_error = 0;  



void add_function(char* name, int param_count, Node* params) {
    printf("DEBUG: Registering function '%s' with %d parameters\n", name, param_count);
    if (is_function_defined(name) != 0) {
        duplicate_function_error = 1;
        yyerror("Error: Function already defined");
        return;
    }
    
    if (function_count < MAX_FUNCTIONS) {
        function_table[function_count].name = strdup(name);
        function_table[function_count].param_count = param_count;
        
        // Allocate and store parameter types
        if (param_count > 0) {
            function_table[function_count].param_types = (char**)malloc(sizeof(char*) * param_count);
            
            // Extract parameter types from params node
            for (int i = 0; i < param_count; i++) {
                // Parse parameter string to extract type
                // Assuming format: "parSOMETHING TYPE NAME"
                char* param_str = params->children[i]->name;
                char* type_start = strchr(param_str, ' ') + 1;
                char* type_end = strchr(type_start, ' ');
                int type_len = type_end - type_start;
                
                char* type = (char*)malloc(type_len + 1);
                strncpy(type, type_start, type_len);
                type[type_len] = '\0';
                
                function_table[function_count].param_types[i] = type;
            }
        } else {
            function_table[function_count].param_types = NULL;
        }
        
        function_count++;
    }
}

void check_param_types(const char* func_name, Node* args_node, int line) {
    int func_index = is_function_defined(func_name);
    if (func_index <= 0) return;  // Function not found (should be caught elsewhere)
    
    func_index--; // Convert to 0-based index
    FunctionInfo* func = &function_table[func_index];
    
    // If no parameters, nothing to check
    if (func->param_count == 0) return;
    
    // If ARGS NONE, no arguments provided
    if (strcmp(args_node->name, "ARGS NONE") == 0) return;
    
    // Check each argument against the parameter type
    for (int i = 0; i < func->param_count && i < args_node->child_count; i++) {
        // Get the argument node
        Node* arg = args_node->children[i];
        
        // Determine the argument type
        char* arg_type = NULL;
        
        // If it's a literal, determine type from the literal
        if (arg->child_count == 0) {
            if (isdigit(arg->name[0]) || (arg->name[0] == '-' && isdigit(arg->name[1]))) {
                if (strchr(arg->name, '.'))
                    arg_type = "REAL";
                else
                    arg_type = "INT";
            }
            else if (arg->name[0] == '\'')
                arg_type = "CHAR";
            else if (arg->name[0] == '"')
                arg_type = "STRING";
            else if (strcmp(arg->name, "TRUE") == 0 || strcmp(arg->name, "FALSE") == 0)
                arg_type = "BOOL";
            else if (strcmp(arg->name, "NULL") == 0)
                arg_type = "NULL";  // Will need special handling
            else {
                // If it's an identifier, try to look up its type
                arg_type = get_var_type(arg->name);
                if (arg_type == NULL) {
                    // If we can't determine the type, just continue - other error handling will catch this
                    continue;
                }
            }
        }
        
        // Debug output to see what's being detected
        if (debug_mode) {
            printf("DEBUG: Argument %d of function '%s' - detected type: '%s', expected type: '%s'\n", 
                   i+1, func_name, arg_type ? arg_type : "unknown", func->param_types[i]);
        }
        
        // For now, basic check for literals
        if (arg_type != NULL && strcasecmp(arg_type, func->param_types[i]) != 0) {
            // Special case for NULL with pointer types
            if (strcmp(arg_type, "NULL") == 0 && strstr(func->param_types[i], "_PTR") != NULL) {
                continue;  // NULL is compatible with pointer types
            }
            
            // Add more debug info
            if (debug_mode) {
                printf("DEBUG: Type mismatch for parameter %d of function '%s'\n", i+1, func_name);
                printf("       Expected: '%s', Got: '%s'\n", func->param_types[i], arg_type);
            }
            
            param_type_mismatch = 1;
            sprintf(yytext, "%s", func_name);
            yylineno = line;
            // Make error message more specific
            char error_msg[256];
            sprintf(error_msg, "Parameter type mismatch in function '%s': expected '%s' for parameter %d, got '%s'", 
                    func_name, func->param_types[i], i+1, arg_type);
            yyerror(error_msg);
            return;  // Exit after first error to avoid multiple errors
        }
    }
}

// Check if function exists - return index+1 (0 if not found)
int is_function_defined(const char* name) {
    for (int i = 0; i < function_count; i++) {
        if (strcmp(function_table[i].name, name) == 0)
            return i + 1;  // Return position + 1 (0 means not found)
    }
    return 0;  // Not found
}

// Get function's parameter count
int get_function_param_count(const char* name) {
    int index = is_function_defined(name);
    if (index > 0)
        return function_table[index-1].param_count;
    return -1;  // Function not found
}

// Check parameter count
void check_param_count(const char* func_name, int provided_count, int line) {
    int required_count = get_function_param_count(func_name);
    if (required_count >= 0 && required_count != provided_count) {
        param_count_mismatch = 1;
        sprintf(yytext, "%s", func_name);  // Store function name for error
        yylineno = line;
        yyerror("Parameter count mismatch");
    }
}

// Check if variable exists in current scope (including function parameters)
// Check if variable exists in current scope
int is_var_defined(const char* name) {
    // More detailed debugging
    // printf("Checking if '%s' is defined (var_count = %d)\n", name, var_count);
    
    // First check if it's a function name
    if (is_function_defined(name) != 0) {
        // printf("'%s' is a function\n", name);
        return 1;  // Consider function names as "defined"
    }
    
    // Then check variable names
    for (int i = 0; i < var_count; i++) {
        if (var_names[i] && strcmp(var_names[i], name) == 0) {
            // printf("'%s' is a defined variable\n", name);
            return 1;
        }
    }
    
    // Special case for function parameters being used in the function body
    if (function_count > 0) {
        FunctionInfo* last_func = &function_table[function_count-1];
        if (last_func->param_count > 0) {
            // Manually check if name matches any parameter by parsing parameter strings
            for (int i = 0; i < var_count; i++) {
                if (var_names[i] && strcmp(var_names[i], name) == 0) {
                    // printf("'%s' is a parameter of the current function\n", name);
                    return 1;
                }
            }
        }
    }
    
    // printf("'%s' is NOT defined\n", name);
    return 0;
}
/*
int check_param_order(const char* param_name, int expected_index) {
    // Extract the number from "parX" format
    if (strncmp(param_name, "par", 3) != 0) {
        param_order_error = 1;
        return 0;
    }
    
    const char* num_part = param_name + 3; // Skip "par" - keep const
    int param_num = atoi(num_part);
    
    if (param_num != expected_index) {
        param_order_error = 1;
        return 0;
    }
    
    return 1;
}*/

// Add variable to current scope
void add_var(char* name, char* type) {
    if (is_var_defined(name)) {
        duplicate_var_error = 1;
        yyerror("Error: Variable already defined in this scope");
        return;
    }
    
    if (var_count < MAX_VARS_PER_SCOPE) {
        var_names[var_count] = strdup(name);
        var_info[var_count].name = strdup(name);
        var_info[var_count].type = strdup(type);
        var_count++;
    }
}

void clear_vars() {
    for (int i = 0; i < var_count; i++) {
        if (var_names[i]) {
            free(var_names[i]);
            var_names[i] = NULL;
        }
        if (var_info[i].name) {
            free(var_info[i].name);
            var_info[i].name = NULL;
        }
        if (var_info[i].type) {
            free(var_info[i].type);
            var_info[i].type = NULL;
        }
    }
    var_count = 0;
}

int check_function_call(const char* name, int line) {
    if (is_function_defined(name) == 0) {
        return 0;  // Function not found
    }
    return 1;  // Function found
}
void check_var_use(const char* name, int line) {
    // Skip check if this is a predefined identifier or in debug mode
    if (strcmp(name, "_main_") == 0 || is_function_defined(name) != 0 || debug_mode) {
        return;  // Skip the check in debug mode
    }
    
    if (!is_var_defined(name)) {
        undeclared_var_error = 1;
        sprintf(yytext, "%s", name);
        yylineno = line;
        yyerror("Variable used before declaration");
    }
}

int is_bool_expr(Node* expr) {
    if (!expr) return 0;
    
    // Direct boolean literals
    if (strcmp(expr->name, "TRUE") == 0 || strcmp(expr->name, "FALSE") == 0)
        return 1;
    
    // Variable of type BOOL
    if (expr->child_count == 0) {
        char* var_type = get_var_type(expr->name);
        if (var_type && strcmp(var_type, "BOOL") == 0)
            return 1;
    }
    
    // Boolean operations
    if (strcmp(expr->name, "AND") == 0 || strcmp(expr->name, "OR") == 0 ||
        strcmp(expr->name, "NOT") == 0 || strcmp(expr->name, "BITWISE_NOT") == 0 ||
        strcmp(expr->name, "BITWISE_OR") == 0 ||
        strcmp(expr->name, "BITWISE_AND") == 0 ||
        strcmp(expr->name, "==") == 0 ||
        strcmp(expr->name, "!=") == 0 || strcmp(expr->name, "<") == 0 ||
        strcmp(expr->name, ">") == 0 || strcmp(expr->name, "<=") == 0 ||
        strcmp(expr->name, ">=") == 0)
        return 1;
    
    return 0;
}

int is_int_expr(Node* expr) {
    if (!expr) return 0;
    
    // Direct integer literals
    if (expr->child_count == 0) {
        // If it's a numeric literal without decimal point
        if (isdigit(expr->name[0]) && !strchr(expr->name, '.'))
            return 1;
        
        // Check if it's a variable identifier of type INT
        char* var_type = get_var_type(expr->name);
        if (var_type && strcmp(var_type, "INT") == 0)
            return 1;
    }
    
    // Arithmetic operations are integer expressions if operands are integers
    if (strcmp(expr->name, "+") == 0 || strcmp(expr->name, "-") == 0 ||
        strcmp(expr->name, "*") == 0 || strcmp(expr->name, "/") == 0) {
        // For simplicity, we'll just return 1 for arithmetic operations
        return 1;
    }
    
    return 0;
}

int is_string_expr(Node* expr) {
    if (!expr) return 0;
    
    // Direct string literals
    if (expr->child_count == 0 && expr->name[0] == '"')
        return 1;
    
    // For this basic check, we'll assume other expressions are not strings
    return 0;
}

char* extract_param_name(const char* param_str) {
    // Assuming format: "parSOMETHING TYPE NAME"
    char* last_space = strrchr(param_str, ' ');
    if (last_space) {
        return last_space + 1;  // Return pointer to the name
    }
    return NULL;  // Error case
}

// Add this function near other is_xxx_defined functions
int is_parameter(const char* name) {
    // Look through the most recently defined function
    if (function_count > 0) {
        FunctionInfo* current_func = &function_table[function_count-1];
        
        // If the function has parameters, check if 'name' is one of them
        if (current_func->param_count > 0 && current_func->param_types != NULL) {
            for (int i = 0; i < var_count; i++) {
                if (strcmp(var_names[i], name) == 0)
                    return 1;
            }
        }
    }
    return 0;
}

char* get_var_type(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(var_info[i].name, name) == 0) {
            return var_info[i].type;
        }
    }
    return NULL;
}

// Debug function - enable when needed
void debug_print_vars(const char* location) {
    printf("=== Variables at %s ===\n", location);
    for (int i = 0; i < var_count; i++) {
        printf("var[%d]: %s\n", i, var_names[i]);
    }
    printf("===========================\n");
}

void debug_node(Node* node, const char* label) {
    printf("DEBUG %s: name='%s', child_count=%d\n", 
           label, node->name, node->child_count);
    if (node->child_count > 0) {
        for (int i = 0; i < node->child_count; i++) {
            printf("  child[%d]: name='%s', child_count=%d\n", 
                   i, node->children[i]->name, node->children[i]->child_count);
        }
    }
}

void debug_print_all_vars() {
    printf("DEBUG: All variables in current scope:\n");
    for (int i = 0; i < var_count; i++) {
        printf("  var[%d]: name='%s', type='%s'\n", 
               i, var_info[i].name, var_info[i].type);
    }
}
// מחסנית טיפוסי חזרה
void push_return_type(const char* type) {
    if (++stack_top < MAX_DEPTH) {
        strcpy(return_type_stack[stack_top], type);
        strcpy(current_return_type, type);
    }
}

void pop_return_type() {
    if (stack_top >= 0) {
        stack_top--;
        if (stack_top >= 0)
            strcpy(current_return_type, return_type_stack[stack_top]);
        else
            current_return_type[0] = '\0';
    }
}

// Add this to your C code section at the top
char* get_expr_type(Node* expr) {
    if (!expr) return NULL;
    
    // Direct literals
    if (expr->child_count == 0) {
        // Integer literal
        if (isdigit(expr->name[0]) && !strchr(expr->name, '.'))
            return "INT";
        
        // Real literal
        if (isdigit(expr->name[0]) && strchr(expr->name, '.'))
            return "REAL";
        
        // Character literal
        if (expr->name[0] == '\'')
            return "CHAR";
        
        // String literal
        if (expr->name[0] == '"')
            return "STRING";
        
        // Boolean literals
        if (strcmp(expr->name, "TRUE") == 0 || strcmp(expr->name, "FALSE") == 0)
            return "BOOL";
        
        // Variable - look up its type
        return get_var_type(expr->name);
    }
    
    // Check operation type
    if (strcmp(expr->name, "*") == 0) {
        // Check if it's unary dereference or binary multiplication based on child count
        if (expr->child_count == 1) {
            // Unary dereference
            char* operand_type = get_expr_type(expr->children[0]);
            
            if (!operand_type) return NULL;
            
            // Operand must be a pointer type
            if (strcmp(operand_type, "INT_PTR") == 0)
                return "INT";
            else if (strcmp(operand_type, "REAL_PTR") == 0)
                return "REAL";
            else if (strcmp(operand_type, "CHAR_PTR") == 0)
                return "CHAR";
            else {
                printf("Type error at line %d: Cannot dereference non-pointer type\n", yylineno);
                return NULL;
            }
        }
        else if (expr->child_count == 2) {
            // Binary multiplication
            char* left_type = get_expr_type(expr->children[0]);
            char* right_type = get_expr_type(expr->children[1]);
            
            if (!left_type || !right_type) return NULL;
            
            // Check if operands are numeric
            if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
                (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
                printf("Type error at line %d: Arithmetic operators require numeric operands\n", yylineno);
                return NULL;
            }
            
            // If either is REAL, result is REAL
            if (strcmp(left_type, "REAL") == 0 || strcmp(right_type, "REAL") == 0)
                return "REAL";
            else
                return "INT";
        }
    }
    
    // Other arithmetic operators
    if (strcmp(expr->name, "+") == 0 || strcmp(expr->name, "-") == 0 || 
        strcmp(expr->name, "/") == 0) {
        char* left_type = get_expr_type(expr->children[0]);
        char* right_type = get_expr_type(expr->children[1]);
        
        if (!left_type || !right_type) return NULL;
        
        // Check if operands are numeric
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            printf("Type error at line %d: Arithmetic operators require numeric operands\n", yylineno);
            return NULL;
        }
        
        // If either is REAL, result is REAL
        if (strcmp(left_type, "REAL") == 0 || strcmp(right_type, "REAL") == 0)
            return "REAL";
        else
            return "INT";
    }
    
    // Logical operators
    if (strcmp(expr->name, "AND") == 0 || strcmp(expr->name, "OR") == 0) {
        char* left_type = get_expr_type(expr->children[0]);
        char* right_type = get_expr_type(expr->children[1]);
        
        if (!left_type || !right_type) return NULL;
        
        // Both operands must be BOOL
        if (strcmp(left_type, "BOOL") != 0 || strcmp(right_type, "BOOL") != 0) {
            printf("Type error at line %d: Logical operators require boolean operands\n", yylineno);
            return NULL;
        }
        
        return "BOOL";
    }
    
    // NOT operator
    if (strcmp(expr->name, "NOT") == 0) {
        char* operand_type = get_expr_type(expr->children[0]);
        
        if (!operand_type) return NULL;
        
        // Operand must be BOOL
        if (strcmp(operand_type, "BOOL") != 0) {
            printf("Type error at line %d: NOT operator requires a boolean operand\n", yylineno);
            return NULL;
        }
        
        return "BOOL";
    }
    
    // BITWISE_NOT operator
    if (strcmp(expr->name, "BITWISE_NOT") == 0) {
        char* operand_type = get_expr_type(expr->children[0]);
        
        if (!operand_type) return NULL;
        
        // Operand must be BOOL
        if (strcmp(operand_type, "BOOL") != 0) {
            printf("Type error at line %d: BITWISE_NOT operator requires a boolean operand\n", yylineno);
            return NULL;
        }
        
        return "BOOL";
    }
    
    // Comparison operators
    if (strcmp(expr->name, "<") == 0 || strcmp(expr->name, ">") == 0 ||
        strcmp(expr->name, "<=") == 0 || strcmp(expr->name, ">=") == 0) {
        char* left_type = get_expr_type(expr->children[0]);
        char* right_type = get_expr_type(expr->children[1]);
        
        if (!left_type || !right_type) return NULL;
        
        // Operands must be numeric
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            printf("Type error at line %d: Comparison operators require numeric operands\n", yylineno);
            return NULL;
        }
        
        return "BOOL";
    }
    
    // Equality operators
    if (strcmp(expr->name, "==") == 0 || strcmp(expr->name, "!=") == 0) {
        char* left_type = get_expr_type(expr->children[0]);
        char* right_type = get_expr_type(expr->children[1]);
        
        if (!left_type || !right_type) return NULL;
        
        // Types must match (or be compatible)
        if (strcmp(left_type, right_type) != 0 &&
            !((strcmp(left_type, "INT") == 0 && strcmp(right_type, "REAL") == 0) ||
              (strcmp(left_type, "REAL") == 0 && strcmp(right_type, "INT") == 0))) {
            printf("Type error at line %d: Equality operators require compatible types\n", yylineno);
            return NULL;
        }
        
        return "BOOL";
    }
    
    // Length operator
    if (strcmp(expr->name, "LEN") == 0) {
        char* operand_type = get_expr_type(expr->children[0]);
        
        if (!operand_type) return NULL;
        
        // Operand must be STRING
        if (strcmp(operand_type, "STRING") != 0) {
            printf("Type error at line %d: Length operator requires a string operand\n", yylineno);
            return NULL;
        }
        
        return "INT";
    }
    
    // Function calls - would need to look up the function's return type
    if (strcmp(expr->name, "CALL") == 0) {
        // For this simple version, we'll just return a valid type
        return "INT"; // Or implement proper function return type lookup
    }
    
    return NULL; // Unknown expression type
}
// בדיקת התאמת סוג RETURN
int check_return_type(Node* expr_node) {
    if (!expr_node) return 1;

    if (current_return_type[0] == '\0') {
        proc_return_error = 1;
        return 0;
    }

    // קריאה לפונקציה תמיד חוקי
    if (strcmp(expr_node->name, "CALL") == 0)
        return 1;

    // משתנה
    if (expr_node->child_count == 0 && !isdigit(expr_node->name[0]) &&
        expr_node->name[0] != '\'' && expr_node->name[0] != '"' &&
        strcmp(expr_node->name, "TRUE") != 0 &&
        strcmp(expr_node->name, "FALSE") != 0 &&
        strcmp(expr_node->name, "NULL") != 0)
        return 1;

    if ((strcmp(expr_node->name, "TRUE") == 0 || strcmp(expr_node->name, "FALSE") == 0) &&
        strcmp(current_return_type, "BOOL") == 0)
        return 1;
    
    if (isdigit(expr_node->name[0]) && strcmp(current_return_type, "INT") == 0)
        return 1;
    
    if (strchr(expr_node->name, '.') && strcmp(current_return_type, "REAL") == 0)
        return 1;
    
    if (expr_node->name[0] == '\'' && strcmp(current_return_type, "CHAR") == 0)
        return 1;
    
    if (expr_node->name[0] == '"' && strcmp(current_return_type, "STRING") == 0)
        return 1;
    
    if (strcmp(expr_node->name, "NULL") == 0 &&
       (strcmp(current_return_type, "INT_PTR") == 0 ||
        strcmp(current_return_type, "REAL_PTR") == 0 ||
        strcmp(current_return_type, "CHAR_PTR") == 0))
        return 1;

    // אופרטורים בסיסיים
    if (strcmp(expr_node->name, "+") == 0 || strcmp(expr_node->name, "-") == 0 ||
        strcmp(expr_node->name, "*") == 0 || strcmp(expr_node->name, "/") == 0)
        return 1;

    return_type_error = 1;
    return 0;
}

// בדיקה אם פונקציה מסתיימת ב-RETURN
int ends_with_return(Node* node) {
    if (!node) return 0;
    if (strcmp(node->name, "BLOCK") == 0 || strcmp(node->name, "BODY") == 0) {
        if (node->child_count == 0) return 0;
        return ends_with_return(node->children[node->child_count - 1]);
    }
    if (strcmp(node->name, "RET") == 0) return 1;
    if (strcmp(node->name, "IF-ELSE") == 0 && node->child_count >= 3) {
        return ends_with_return(node->children[1]) && ends_with_return(node->children[2]);
    }
    return 0;
}

#line 805 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_DEF = 3,                        /* DEF  */
  YYSYMBOL_RETURNS = 4,                    /* RETURNS  */
  YYSYMBOL_RETURN = 5,                     /* RETURN  */
  YYSYMBOL_IF = 6,                         /* IF  */
  YYSYMBOL_ELSE = 7,                       /* ELSE  */
  YYSYMBOL_ELIF = 8,                       /* ELIF  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_FOR = 10,                       /* FOR  */
  YYSYMBOL_DO = 11,                        /* DO  */
  YYSYMBOL_VAR = 12,                       /* VAR  */
  YYSYMBOL_BEGIN_T = 13,                   /* BEGIN_T  */
  YYSYMBOL_END_T = 14,                     /* END_T  */
  YYSYMBOL_CALL = 15,                      /* CALL  */
  YYSYMBOL_NULL_T = 16,                    /* NULL_T  */
  YYSYMBOL_TRUE = 17,                      /* TRUE  */
  YYSYMBOL_FALSE = 18,                     /* FALSE  */
  YYSYMBOL_TYPE = 19,                      /* TYPE  */
  YYSYMBOL_INT = 20,                       /* INT  */
  YYSYMBOL_REAL = 21,                      /* REAL  */
  YYSYMBOL_CHAR = 22,                      /* CHAR  */
  YYSYMBOL_BOOL = 23,                      /* BOOL  */
  YYSYMBOL_STRING = 24,                    /* STRING  */
  YYSYMBOL_INT_PTR = 25,                   /* INT_PTR  */
  YYSYMBOL_REAL_PTR = 26,                  /* REAL_PTR  */
  YYSYMBOL_CHAR_PTR = 27,                  /* CHAR_PTR  */
  YYSYMBOL_AND = 28,                       /* AND  */
  YYSYMBOL_OR = 29,                        /* OR  */
  YYSYMBOL_NOT = 30,                       /* NOT  */
  YYSYMBOL_EQ = 31,                        /* EQ  */
  YYSYMBOL_NE = 32,                        /* NE  */
  YYSYMBOL_LE = 33,                        /* LE  */
  YYSYMBOL_GE = 34,                        /* GE  */
  YYSYMBOL_LT = 35,                        /* LT  */
  YYSYMBOL_GT = 36,                        /* GT  */
  YYSYMBOL_ASSIGN = 37,                    /* ASSIGN  */
  YYSYMBOL_ADD = 38,                       /* ADD  */
  YYSYMBOL_SUB = 39,                       /* SUB  */
  YYSYMBOL_MUL = 40,                       /* MUL  */
  YYSYMBOL_DIV = 41,                       /* DIV  */
  YYSYMBOL_ADDR = 42,                      /* ADDR  */
  YYSYMBOL_DEREF = 43,                     /* DEREF  */
  YYSYMBOL_PIPE_SYMBOL = 44,               /* PIPE_SYMBOL  */
  YYSYMBOL_IDENTIFIER = 45,                /* IDENTIFIER  */
  YYSYMBOL_INT_LITERAL = 46,               /* INT_LITERAL  */
  YYSYMBOL_REAL_LITERAL = 47,              /* REAL_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 48,              /* CHAR_LITERAL  */
  YYSYMBOL_STRING_LITERAL = 49,            /* STRING_LITERAL  */
  YYSYMBOL_BITWISE_OR = 50,                /* BITWISE_OR  */
  YYSYMBOL_BITWISE_AND = 51,               /* BITWISE_AND  */
  YYSYMBOL_BITWISE_NOT = 52,               /* BITWISE_NOT  */
  YYSYMBOL_UMINUS = 53,                    /* UMINUS  */
  YYSYMBOL_LOWER_THAN_ELSE = 54,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_55_ = 55,                       /* '('  */
  YYSYMBOL_56_ = 56,                       /* ')'  */
  YYSYMBOL_57_ = 57,                       /* ':'  */
  YYSYMBOL_58_ = 58,                       /* ';'  */
  YYSYMBOL_59_ = 59,                       /* '['  */
  YYSYMBOL_60_ = 60,                       /* ']'  */
  YYSYMBOL_61_ = 61,                       /* ','  */
  YYSYMBOL_YYACCEPT = 62,                  /* $accept  */
  YYSYMBOL_program = 63,                   /* program  */
  YYSYMBOL_funcs = 64,                     /* funcs  */
  YYSYMBOL_func = 65,                      /* func  */
  YYSYMBOL_nested_func = 66,               /* nested_func  */
  YYSYMBOL_func_header = 67,               /* func_header  */
  YYSYMBOL_parameters = 68,                /* parameters  */
  YYSYMBOL_parameter = 69,                 /* parameter  */
  YYSYMBOL_ret_type = 70,                  /* ret_type  */
  YYSYMBOL_type = 71,                      /* type  */
  YYSYMBOL_block = 72,                     /* block  */
  YYSYMBOL_body = 73,                      /* body  */
  YYSYMBOL_inner_block = 74,               /* inner_block  */
  YYSYMBOL_stmts = 75,                     /* stmts  */
  YYSYMBOL_stmt = 76,                      /* stmt  */
  YYSYMBOL_var_decls = 77,                 /* var_decls  */
  YYSYMBOL_optional_var_list = 78,         /* optional_var_list  */
  YYSYMBOL_var_decl_list = 79,             /* var_decl_list  */
  YYSYMBOL_var_single_decl = 80,           /* var_single_decl  */
  YYSYMBOL_id_list = 81,                   /* id_list  */
  YYSYMBOL_var_init_list = 82,             /* var_init_list  */
  YYSYMBOL_string_decl_list = 83,          /* string_decl_list  */
  YYSYMBOL_string_decl = 84,               /* string_decl  */
  YYSYMBOL_expr = 85,                      /* expr  */
  YYSYMBOL_args = 86                       /* args  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1200

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  62
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  121
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  280

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   309


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      55,    56,     2,     2,    61,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    57,    58,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    59,     2,    60,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   774,   774,   778,   779,   780,   784,   796,   808,   837,
     879,   906,   915,   921,   927,   935,   936,   937,   938,   939,
     940,   941,   942,   951,   952,   953,   954,   955,   956,   957,
     958,   962,   963,   968,   969,   973,   984,   995,   996,   997,
    1001,  1002,  1016,  1023,  1029,  1033,  1067,  1098,  1143,  1163,
    1170,  1177,  1184,  1191,  1198,  1205,  1213,  1216,  1219,  1222,
    1228,  1238,  1239,  1243,  1244,  1248,  1255,  1259,  1276,  1300,
    1309,  1313,  1320,  1348,  1351,  1372,  1380,  1397,  1404,  1410,
    1416,  1426,  1444,  1462,  1480,  1498,  1517,  1536,  1554,  1572,
    1590,  1609,  1623,  1637,  1651,  1665,  1678,  1694,  1695,  1696,
    1717,  1724,  1730,  1752,  1767,  1772,  1788,  1792,  1823,  1850,
    1854,  1855,  1856,  1857,  1858,  1859,  1860,  1861,  1873,  1882,
    1883,  1894
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "DEF", "RETURNS",
  "RETURN", "IF", "ELSE", "ELIF", "WHILE", "FOR", "DO", "VAR", "BEGIN_T",
  "END_T", "CALL", "NULL_T", "TRUE", "FALSE", "TYPE", "INT", "REAL",
  "CHAR", "BOOL", "STRING", "INT_PTR", "REAL_PTR", "CHAR_PTR", "AND", "OR",
  "NOT", "EQ", "NE", "LE", "GE", "LT", "GT", "ASSIGN", "ADD", "SUB", "MUL",
  "DIV", "ADDR", "DEREF", "PIPE_SYMBOL", "IDENTIFIER", "INT_LITERAL",
  "REAL_LITERAL", "CHAR_LITERAL", "STRING_LITERAL", "BITWISE_OR",
  "BITWISE_AND", "BITWISE_NOT", "UMINUS", "LOWER_THAN_ELSE", "'('", "')'",
  "':'", "';'", "'['", "']'", "','", "$accept", "program", "funcs", "func",
  "nested_func", "func_header", "parameters", "parameter", "ret_type",
  "type", "block", "body", "inner_block", "stmts", "stmt", "var_decls",
  "optional_var_list", "var_decl_list", "var_single_decl", "id_list",
  "var_init_list", "string_decl_list", "string_decl", "expr", "args", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-120)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       8,   -32,    33,  -120,     8,    35,     0,  -120,  -120,    26,
      54,    30,   155,  -120,    26,  -120,    11,  -120,    38,    40,
      52,  -120,  -120,  -120,    57,  -120,  -120,  -120,  -120,    78,
    -120,   180,   247,   247,    62,    79,    26,  -120,    82,    97,
     -30,   101,   128,    35,  -120,   133,   128,   150,  -120,   104,
      95,    96,    30,   112,   113,  -120,  -120,  -120,   247,   247,
     -13,   -11,   119,    -2,  -120,  -120,  -120,  -120,   247,   267,
    -120,   580,   860,   887,   247,    91,   150,   111,   130,   227,
     247,    70,  -120,    54,  -120,  -120,  -120,  -120,   124,   166,
    -120,   -49,   114,    -7,   126,    15,  -120,  -120,  -120,  -120,
     247,   127,   247,   143,   247,   247,  -120,   -20,   941,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,  -120,    91,    91,   608,    26,  -120,   162,
    -120,  -120,   247,   247,   144,   636,   340,   247,   247,  -120,
    -120,   314,   247,  -120,   146,  -120,   247,   142,   148,  -120,
    -120,   149,   967,   247,   993,  -120,   292,   145,   370,   247,
     247,   247,  -120,   215,  1149,    85,    85,    60,    60,    60,
      60,    72,    72,  -120,  -120,  1149,   215,   137,  -120,   247,
      54,   247,   147,   664,   152,  -120,   163,  1019,  1045,  -120,
    -120,  -120,  -120,  -120,  -120,  -120,  -120,  -120,   316,   141,
     692,   153,   154,  -120,   164,  -120,  -120,   400,  -120,   247,
    -120,  -120,  1071,  1097,   -10,   151,   247,   720,  -120,   748,
     156,  -120,   247,   247,   171,   172,   157,  -120,  -120,   176,
    -120,  -120,   175,   177,   178,    91,   914,   247,  -120,  -120,
     165,   776,   247,   247,   182,  -120,   160,   247,   247,   247,
    -120,    91,  1123,   194,  -120,   430,   460,  -120,   490,   520,
     550,   197,   201,  -120,   222,   223,  -120,  -120,  -120,   204,
      91,   247,   247,    91,  -120,   804,   832,  -120,  -120,  -120
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,     0,     2,     3,    62,     0,     1,     4,    64,
       0,    12,     0,    61,    63,    66,     0,     6,     0,     0,
      11,    23,    24,    25,     0,    27,    28,    29,    30,     0,
      65,     0,     0,     0,     0,     0,    64,    32,     0,     0,
       0,     0,    37,    62,    57,     0,    38,    40,    26,     0,
       0,     0,    12,     0,     0,   116,   114,   115,     0,     0,
       0,     0,     0,   106,   110,   111,   112,   113,     0,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    35,     0,    31,    36,    41,    14,     0,     9,
      10,     0,     0,    73,     0,     0,    78,    95,    98,    99,
       0,   102,     0,     0,   121,     0,    96,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    42,     0,     0,     0,    64,    34,     0,
      56,    58,   121,     0,     0,     0,     0,     0,     0,     7,
      13,     0,     0,    71,     0,    70,     0,     0,     0,    68,
      72,     0,     0,     0,     0,   117,   119,     0,     0,     0,
       0,     0,    97,    91,    93,    85,    86,    90,    88,    89,
      87,    81,    82,    83,    84,    94,    92,    52,    53,     0,
       0,     0,     0,     0,     0,    44,     0,     0,     0,    15,
      16,    17,    18,    22,    19,    20,    21,     8,    75,     0,
       0,     0,    73,    74,     0,    77,   105,     0,   104,   121,
     107,   108,     0,     0,    81,     0,     0,     0,    33,     0,
       0,    48,   121,     0,     0,     0,     0,    67,    69,    79,
     103,   120,     0,     0,     0,     0,     0,     0,    54,    46,
       0,     0,     0,     0,     0,    76,     0,     0,     0,     0,
      49,     0,     0,     0,    47,     0,     0,    80,     0,     0,
       0,    51,     0,    45,     0,     0,   100,   101,   118,     0,
       0,     0,     0,     0,    55,     0,     0,    50,    59,    60
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -120,  -120,   258,  -120,  -120,    68,   216,  -120,  -120,   252,
      -6,  -119,    51,   231,   205,   237,   -33,  -120,   274,   169,
      64,  -120,   159,   -31,  -117
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     4,    42,    43,    19,    20,   197,    29,
     128,   129,    45,    46,    47,    10,    13,    14,    15,    94,
      92,    95,    96,   156,   157
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      71,    72,    73,    76,    17,   177,   178,    79,   142,   143,
      44,     1,   144,     6,     1,   182,    31,    32,   159,   160,
      33,    34,    35,    36,    16,    37,    38,    97,    98,    80,
     119,   120,    99,     7,   101,   104,    44,   106,   108,   105,
      44,    44,   100,   126,   102,    12,   234,     9,   135,   136,
     146,    39,   147,   104,   148,    11,    40,   105,    21,    22,
      23,    48,    25,    26,    27,    28,    41,    16,     5,   152,
     130,   154,     5,   150,   158,    18,   151,   139,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   231,    82,   180,    49,    51,    85,   117,   118,
     119,   120,   183,   127,    16,   240,   187,   188,   137,   138,
      52,   198,   119,   120,    53,   200,   250,    74,   113,   114,
     115,   116,   207,   117,   118,   119,   120,    77,   212,   213,
     214,     1,   261,    31,    32,    54,    75,    33,    34,    35,
      36,    16,    78,    38,   215,   216,    81,    84,   217,    87,
     219,   274,    88,    89,   277,    31,    32,    91,    93,    33,
      34,    35,    36,    16,   103,    38,   132,   133,    39,   140,
     141,   181,   145,    40,   218,    21,    22,    23,    24,    25,
      26,    27,    28,    41,   149,   236,   153,   155,   201,   184,
      39,   199,   241,   202,   204,    40,    55,    56,    57,   227,
     223,   210,   244,   220,   269,    41,   252,   222,   235,   257,
      58,   255,   256,   229,   239,   148,   258,   259,   260,    59,
      60,   253,    61,   147,    62,    63,    64,    65,    66,    67,
     242,   243,    68,   246,   247,    69,   248,   249,    70,   142,
     275,   276,   134,    55,    56,    57,   111,   112,   113,   114,
     115,   116,   263,   117,   118,   119,   120,    58,   270,   271,
     272,   273,     8,    55,    56,    57,    59,    60,    90,    61,
      50,    62,    63,    64,    65,    66,    67,    58,    86,    68,
      83,   131,    69,    55,    56,    57,    59,    60,    30,    61,
     245,    62,    63,    64,    65,    66,    67,    58,     0,    68,
       0,     0,    69,     0,     0,     0,    59,    60,     0,    61,
     205,    62,   107,    64,    65,    66,    67,   203,     0,    68,
     109,   110,    69,   111,   112,   113,   114,   115,   116,     0,
     117,   118,   119,   120,   189,   190,   191,   192,   193,   194,
     195,   196,   121,   122,   109,   110,     0,   111,   112,   113,
     114,   115,   116,   209,   117,   118,   119,   120,     0,     0,
       0,     0,     0,     0,     0,     0,   121,   122,   109,   110,
       0,   111,   112,   113,   114,   115,   116,   226,   117,   118,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,     0,     0,     0,     0,   109,   110,
     186,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,     0,     0,     0,     0,   109,   110,
     211,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,     0,     0,     0,     0,   109,   110,
     230,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,     0,     0,     0,     0,   109,   110,
     264,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,     0,     0,     0,     0,   109,   110,
     265,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,     0,     0,     0,     0,   109,   110,
     266,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,     0,     0,     0,     0,   109,   110,
     267,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,     0,     0,     0,     0,   109,   110,
     268,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,     0,     0,   109,   110,   123,   111,
     112,   113,   114,   115,   116,     0,   117,   118,   119,   120,
       0,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,     0,     0,   109,   110,   179,   111,   112,   113,
     114,   115,   116,     0,   117,   118,   119,   120,     0,     0,
       0,     0,     0,     0,     0,     0,   121,   122,     0,     0,
       0,     0,   109,   110,   185,   111,   112,   113,   114,   115,
     116,     0,   117,   118,   119,   120,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,     0,     0,     0,
     109,   110,   221,   111,   112,   113,   114,   115,   116,     0,
     117,   118,   119,   120,     0,     0,     0,     0,     0,     0,
       0,     0,   121,   122,     0,     0,     0,     0,   109,   110,
     228,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,     0,     0,   109,   110,   237,   111,
     112,   113,   114,   115,   116,     0,   117,   118,   119,   120,
       0,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,     0,     0,   109,   110,   238,   111,   112,   113,
     114,   115,   116,     0,   117,   118,   119,   120,     0,     0,
       0,     0,     0,     0,     0,     0,   121,   122,     0,     0,
       0,     0,   109,   110,   254,   111,   112,   113,   114,   115,
     116,     0,   117,   118,   119,   120,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,     0,     0,     0,
     109,   110,   278,   111,   112,   113,   114,   115,   116,     0,
     117,   118,   119,   120,     0,     0,     0,     0,     0,     0,
       0,     0,   121,   122,     0,     0,     0,     0,   109,   110,
     279,   111,   112,   113,   114,   115,   116,     0,   117,   118,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,     0,   109,   110,   124,   111,   112,
     113,   114,   115,   116,     0,   117,   118,   119,   120,     0,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,     0,   109,   110,   125,   111,   112,   113,   114,   115,
     116,     0,   117,   118,   119,   120,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,     0,     0,   109,
     110,   251,   111,   112,   113,   114,   115,   116,     0,   161,
     118,   119,   120,     0,     0,     0,     0,     0,     0,     0,
       0,   121,   122,     0,     0,   109,   110,   162,   111,   112,
     113,   114,   115,   116,     0,   117,   118,   119,   120,     0,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,   109,   110,   206,   111,   112,   113,   114,   115,   116,
       0,   117,   118,   119,   120,     0,     0,     0,     0,     0,
       0,     0,     0,   121,   122,     0,     0,   109,   110,   208,
     111,   112,   113,   114,   115,   116,     0,   117,   118,   119,
     120,     0,     0,     0,     0,     0,     0,     0,     0,   121,
     122,     0,     0,   109,   110,   224,   111,   112,   113,   114,
     115,   116,     0,   117,   118,   119,   120,     0,     0,     0,
       0,     0,     0,     0,     0,   121,   122,     0,     0,   109,
     110,   225,   111,   112,   113,   114,   115,   116,     0,   117,
     118,   119,   120,     0,     0,     0,     0,     0,     0,     0,
       0,   121,   122,     0,     0,   109,   110,   232,   111,   112,
     113,   114,   115,   116,     0,   117,   118,   119,   120,     0,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,   109,   110,   233,   111,   112,   113,   114,   115,   116,
       0,   117,   118,   119,   120,     0,     0,     0,     0,     0,
       0,     0,     0,   121,   122,     0,     0,   109,     0,   262,
     111,   112,   113,   114,   115,   116,     0,   117,   118,   119,
     120,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     122
};

static const yytype_int16 yycheck[] =
{
      31,    32,    33,    36,    10,   124,   125,    37,    57,    58,
      16,     3,    61,    45,     3,   132,     5,     6,    38,    39,
       9,    10,    11,    12,    13,    14,    15,    58,    59,    59,
      40,    41,    45,     0,    45,    55,    42,    68,    69,    59,
      46,    47,    55,    74,    55,    19,    56,    12,    79,    80,
      57,    40,    59,    55,    61,    55,    45,    59,    20,    21,
      22,    23,    24,    25,    26,    27,    55,    13,     0,   100,
      76,   102,     4,    58,   105,    45,    61,    83,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   209,    42,   127,    57,    56,    46,    38,    39,
      40,    41,   133,    12,    13,   222,   137,   138,    38,    39,
      58,   142,    40,    41,    57,   146,   235,    55,    33,    34,
      35,    36,   153,    38,    39,    40,    41,    45,   159,   160,
     161,     3,   251,     5,     6,    57,    57,     9,    10,    11,
      12,    13,    45,    15,     7,     8,    45,    14,   179,    45,
     181,   270,    57,    57,   273,     5,     6,    45,    45,     9,
      10,    11,    12,    13,    45,    15,    55,    37,    40,    45,
       4,     9,    58,    45,   180,    20,    21,    22,    23,    24,
      25,    26,    27,    55,    58,   216,    59,    44,    46,    45,
      40,    45,   223,    45,    45,    45,    16,    17,    18,    58,
      37,    56,    45,    56,     7,    55,   237,    55,    57,    49,
      30,   242,   243,    60,    58,    61,   247,   248,   249,    39,
      40,    56,    42,    59,    44,    45,    46,    47,    48,    49,
      59,    59,    52,    57,    59,    55,    59,    59,    58,    57,
     271,   272,    15,    16,    17,    18,    31,    32,    33,    34,
      35,    36,    58,    38,    39,    40,    41,    30,    57,    37,
      37,    57,     4,    16,    17,    18,    39,    40,    52,    42,
      18,    44,    45,    46,    47,    48,    49,    30,    47,    52,
      43,    76,    55,    16,    17,    18,    39,    40,    14,    42,
     226,    44,    45,    46,    47,    48,    49,    30,    -1,    52,
      -1,    -1,    55,    -1,    -1,    -1,    39,    40,    -1,    42,
     151,    44,    45,    46,    47,    48,    49,   148,    -1,    52,
      28,    29,    55,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    40,    41,    20,    21,    22,    23,    24,    25,
      26,    27,    50,    51,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    61,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    28,    29,
      -1,    31,    32,    33,    34,    35,    36,    61,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      60,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      60,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      60,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      60,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      60,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      60,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      60,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      60,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    28,    29,    58,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      -1,    -1,    -1,    -1,    28,    29,    58,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    -1,    -1,
      -1,    -1,    28,    29,    58,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,
      28,    29,    58,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    28,    29,
      58,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    28,    29,    58,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      -1,    -1,    -1,    -1,    28,    29,    58,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    -1,    -1,
      -1,    -1,    28,    29,    58,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,
      28,    29,    58,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    28,    29,
      58,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    28,    29,    57,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    -1,
      -1,    -1,    28,    29,    57,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    28,
      29,    57,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    51,    -1,    -1,    28,    29,    56,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    -1,
      -1,    28,    29,    56,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    51,    -1,    -1,    28,    29,    56,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    -1,    -1,    28,    29,    56,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    51,    -1,    -1,    28,
      29,    56,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    51,    -1,    -1,    28,    29,    56,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    -1,
      -1,    28,    29,    56,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    51,    -1,    -1,    28,    -1,    56,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      51
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    63,    64,    65,    67,    45,     0,    64,    12,
      77,    55,    19,    78,    79,    80,    13,    72,    45,    68,
      69,    20,    21,    22,    23,    24,    25,    26,    27,    71,
      80,     5,     6,     9,    10,    11,    12,    14,    15,    40,
      45,    55,    66,    67,    72,    74,    75,    76,    23,    57,
      71,    56,    58,    57,    57,    16,    17,    18,    30,    39,
      40,    42,    44,    45,    46,    47,    48,    49,    52,    55,
      58,    85,    85,    85,    55,    57,    78,    45,    45,    37,
      59,    45,    74,    77,    14,    74,    75,    45,    57,    57,
      68,    45,    82,    45,    81,    83,    84,    85,    85,    45,
      55,    45,    55,    45,    55,    59,    85,    45,    85,    28,
      29,    31,    32,    33,    34,    35,    36,    38,    39,    40,
      41,    50,    51,    58,    57,    57,    85,    12,    72,    73,
      72,    76,    55,    37,    15,    85,    85,    38,    39,    72,
      45,     4,    57,    58,    61,    58,    57,    59,    61,    58,
      58,    61,    85,    59,    85,    44,    85,    86,    85,    38,
      39,    38,    56,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    73,    73,    58,
      78,     9,    86,    85,    45,    58,    60,    85,    85,    20,
      21,    22,    23,    24,    25,    26,    27,    70,    85,    45,
      85,    46,    45,    81,    45,    84,    56,    85,    56,    61,
      56,    60,    85,    85,    85,     7,     8,    85,    72,    85,
      56,    58,    55,    37,    56,    56,    61,    58,    58,    60,
      60,    86,    56,    56,    56,    57,    85,    58,    58,    58,
      86,    85,    59,    59,    45,    82,    57,    59,    59,    59,
      73,    57,    85,    56,    58,    85,    85,    49,    85,    85,
      85,    73,    56,    58,    60,    60,    60,    60,    60,     7,
      57,    37,    37,    57,    73,    85,    85,    73,    58,    58
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    62,    63,    64,    64,    64,    65,    66,    67,    67,
      68,    68,    68,    69,    69,    70,    70,    70,    70,    70,
      70,    70,    70,    71,    71,    71,    71,    71,    71,    71,
      71,    72,    72,    73,    73,    74,    74,    74,    74,    74,
      75,    75,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    77,    77,    78,    78,    79,    79,    80,    80,    80,
      80,    80,    80,    81,    81,    82,    82,    83,    83,    84,
      84,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    86,
      86,    86
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     0,     3,     3,     8,     6,
       3,     1,     0,     4,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     2,     3,     1,     2,     2,     1,     1,     0,
       1,     2,     3,     2,     4,     8,     6,     7,     5,     7,
      11,     8,     4,     4,     6,    10,     3,     1,     3,    11,
      11,     2,     0,     1,     0,     2,     1,     7,     5,     7,
       5,     5,     5,     1,     3,     3,     5,     3,     1,     4,
       6,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     3,     2,     2,
       8,     8,     2,     5,     4,     4,     1,     4,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     8,     1,
       3,     0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: funcs  */
#line 774 "parser.y"
          { root = create_node("CODE", 1, (yyvsp[0].node)); }
#line 2357 "parser.tab.c"
    break;

  case 3: /* funcs: func  */
#line 778 "parser.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 2363 "parser.tab.c"
    break;

  case 4: /* funcs: func funcs  */
#line 779 "parser.y"
               { (yyval.node) = create_node("FUNC", 2, (yyvsp[-1].node), (yyvsp[0].node)); }
#line 2369 "parser.tab.c"
    break;

  case 5: /* funcs: %empty  */
#line 780 "parser.y"
                { (yyval.node) = NULL; }
#line 2375 "parser.tab.c"
    break;

  case 6: /* func: func_header var_decls block  */
#line 784 "parser.y"
                                {
      if (!ends_with_return((yyvsp[0].node)) && strlen(current_return_type) > 0) {
          missing_return_error = 1;
          yyerror("Function with return type must end with a return statement");
      }
      (yyval.node) = create_node((yyvsp[-2].node)->name, 3, (yyvsp[-2].node)->children[0], (yyvsp[-2].node)->children[1], create_node("BODY", 2, (yyvsp[-1].node), (yyvsp[0].node)));
      pop_return_type();
      clear_vars(); // Clear variables after the function is fully processed
  }
#line 2389 "parser.tab.c"
    break;

  case 7: /* nested_func: func_header var_decls block  */
#line 796 "parser.y"
                                {
      if (!ends_with_return((yyvsp[0].node)) && strlen(current_return_type) > 0) {
          missing_return_error = 1;
          yyerror("Function with return type must end with a return statement");
      }
      (yyval.node) = create_node((yyvsp[-2].node)->name, 3, (yyvsp[-2].node)->children[0], (yyvsp[-2].node)->children[1], create_node("BODY", 2, (yyvsp[-1].node), (yyvsp[0].node)));
      pop_return_type();
      clear_vars(); // Clear variables after the function is fully processed
  }
#line 2403 "parser.tab.c"
    break;

  case 8: /* func_header: DEF IDENTIFIER '(' parameters ')' ':' RETURNS ret_type  */
#line 808 "parser.y"
                                                           {
      param_count = 0;
      push_return_type(return_type_to_push);
      Node* params = (yyvsp[-4].node);
      Node* ret_type_node = (yyvsp[0].node);
      if (strcmp((yyvsp[-6].str), "_main_") == 0) {
          yyerror("Error: _main_() cannot return a value");
      }
	 if (strcmp(params->name, "PARS NONE") != 0) {
          param_count = params->child_count;
          // Add parameters to variable list
          for (int i = 0; i < param_count; i++) {
              // Parse parameter string to extract type
              char* param_str = params->children[i]->name;
              char* type_start = strchr(param_str, ' ') + 1;
              char* type_end = strchr(type_start, ' ');
              int type_len = type_end - type_start;
              
              char type[50];
              strncpy(type, type_start, type_len);
              type[type_len] = '\0';
              
              char* name_start = strrchr(param_str, ' ') + 1;
              add_var(name_start, type); 
          }
      }
      add_function((yyvsp[-6].str), param_count, params);
      (yyval.node) = create_node((yyvsp[-6].str), 2, params, ret_type_node);
  }
#line 2437 "parser.tab.c"
    break;

  case 9: /* func_header: DEF IDENTIFIER '(' parameters ')' ':'  */
#line 837 "parser.y"
                                          {
      param_count = 0;
      push_return_type(""); 
      Node* params = (yyvsp[-2].node);
      Node* ret_type_node = create_node("RET NONE", 0);
      
      // Check if this is the main function
      if (strcmp((yyvsp[-4].str), "_main_") == 0) {
          has_main = 1;  // Set the flag that we've found main
          
          // Check if main has parameters
          if (strcmp(params->name, "PARS NONE") != 0) {
              main_with_params_error = 1;
              yyerror("Error: _main_() cannot have parameters");
          }
      }
      
      if (strcmp(params->name, "PARS NONE") != 0) {
          param_count = params->child_count;
          // Add parameters to variable list
          for (int i = 0; i < param_count; i++) {
              // Parse parameter string to extract type
              char* param_str = params->children[i]->name;
              char* type_start = strchr(param_str, ' ') + 1;
              char* type_end = strchr(type_start, ' ');
              int type_len = type_end - type_start;
              
              char type[50];
              strncpy(type, type_start, type_len);
              type[type_len] = '\0';
              
              char* name_start = strrchr(param_str, ' ') + 1;
              add_var(name_start, type);  // Pass type here
          }
      }
      
      add_function((yyvsp[-4].str), param_count, params);
      (yyval.node) = create_node((yyvsp[-4].str), 2, params, ret_type_node);
  }
#line 2481 "parser.tab.c"
    break;

  case 10: /* parameters: parameter ';' parameters  */
#line 879 "parser.y"
                             {
      Node* pars = create_node("PARS", (yyvsp[0].node)->child_count + 1);
      pars->children[0] = (yyvsp[-2].node);
      
      // Check if parameter is in correct order
      char* param_str = (yyvsp[-2].node)->name;
      int param_index = 1; // First parameter should be par1
      /*
      if (!check_param_order(param_str, param_index)) {
          // Just set the flag, don't exit
          param_order_error = 1;
      }*/
      
      for (int i = 0; i < (yyvsp[0].node)->child_count; i++) {
          pars->children[i+1] = (yyvsp[0].node)->children[i];
          
          // Check each parameter in the list
          param_str = (yyvsp[0].node)->children[i]->name;
          param_index = i + 2; // Next parameters should be par2, par3, etc.
          /*
          if (!check_param_order(param_str, param_index)) {
              // Just set the flag, don't exit
              param_order_error = 1;
          }*/
      }
      (yyval.node) = pars;
  }
#line 2513 "parser.tab.c"
    break;

  case 11: /* parameters: parameter  */
#line 906 "parser.y"
              {
      // Check if this single parameter is par1
      char* param_str = (yyvsp[0].node)->name;/*
      if (!check_param_order(param_str, 1)) {
          // Just set the flag, don't exit
          param_order_error = 1;
      }*/
      (yyval.node) = create_node("PARS", 1, (yyvsp[0].node));
  }
#line 2527 "parser.tab.c"
    break;

  case 12: /* parameters: %empty  */
#line 915 "parser.y"
                {
      (yyval.node) = create_node("PARS NONE", 0);
  }
#line 2535 "parser.tab.c"
    break;

  case 13: /* parameter: IDENTIFIER type ':' IDENTIFIER  */
#line 921 "parser.y"
                                   {
      char temp[100];
      sprintf(temp, "par%s %s %s", (yyvsp[-3].str) + 3, (yyvsp[-2].node)->name, (yyvsp[0].str));
      (yyval.node) = create_node(temp, 0);
      // We'll add the variable when processing all parameters in func_header
  }
#line 2546 "parser.tab.c"
    break;

  case 14: /* parameter: IDENTIFIER ':' IDENTIFIER  */
#line 927 "parser.y"
                              {
      param_error = 1;
      yyerror("no type defined");
      (yyval.node) = create_node("ERROR", 0);
  }
#line 2556 "parser.tab.c"
    break;

  case 15: /* ret_type: INT  */
#line 935 "parser.y"
        { strcpy(return_type_to_push, "INT"); (yyval.node) = create_node("INT", 0); }
#line 2562 "parser.tab.c"
    break;

  case 16: /* ret_type: REAL  */
#line 936 "parser.y"
         { strcpy(return_type_to_push, "REAL"); (yyval.node) = create_node("REAL", 0); }
#line 2568 "parser.tab.c"
    break;

  case 17: /* ret_type: CHAR  */
#line 937 "parser.y"
         { strcpy(return_type_to_push, "CHAR"); (yyval.node) = create_node("CHAR", 0); }
#line 2574 "parser.tab.c"
    break;

  case 18: /* ret_type: BOOL  */
#line 938 "parser.y"
         { strcpy(return_type_to_push, "BOOL"); (yyval.node) = create_node("BOOL", 0); }
#line 2580 "parser.tab.c"
    break;

  case 19: /* ret_type: INT_PTR  */
#line 939 "parser.y"
            { strcpy(return_type_to_push, "INT_PTR"); (yyval.node) = create_node("INT_PTR", 0); }
#line 2586 "parser.tab.c"
    break;

  case 20: /* ret_type: REAL_PTR  */
#line 940 "parser.y"
             { strcpy(return_type_to_push, "REAL_PTR"); (yyval.node) = create_node("REAL_PTR", 0); }
#line 2592 "parser.tab.c"
    break;

  case 21: /* ret_type: CHAR_PTR  */
#line 941 "parser.y"
             { strcpy(return_type_to_push, "CHAR_PTR"); (yyval.node) = create_node("CHAR_PTR", 0); }
#line 2598 "parser.tab.c"
    break;

  case 22: /* ret_type: STRING  */
#line 942 "parser.y"
           { 
      string_return_error = 1;
      yyerror("Error: Functions cannot return type string");
      strcpy(return_type_to_push, ""); // Set empty to avoid further errors
      (yyval.node) = create_node("ERROR", 0); 
    }
#line 2609 "parser.tab.c"
    break;

  case 23: /* type: INT  */
#line 951 "parser.y"
        { (yyval.node) = create_node("INT", 0); }
#line 2615 "parser.tab.c"
    break;

  case 24: /* type: REAL  */
#line 952 "parser.y"
         { (yyval.node) = create_node("REAL", 0); }
#line 2621 "parser.tab.c"
    break;

  case 25: /* type: CHAR  */
#line 953 "parser.y"
         { (yyval.node) = create_node("CHAR", 0); }
#line 2627 "parser.tab.c"
    break;

  case 26: /* type: BOOL  */
#line 954 "parser.y"
         { (yyval.node) = create_node("BOOL", 0); }
#line 2633 "parser.tab.c"
    break;

  case 27: /* type: STRING  */
#line 955 "parser.y"
           { (yyval.node) = create_node("STRING", 0); }
#line 2639 "parser.tab.c"
    break;

  case 28: /* type: INT_PTR  */
#line 956 "parser.y"
            { (yyval.node) = create_node("INT_PTR", 0); }
#line 2645 "parser.tab.c"
    break;

  case 29: /* type: REAL_PTR  */
#line 957 "parser.y"
             { (yyval.node) = create_node("REAL_PTR", 0); }
#line 2651 "parser.tab.c"
    break;

  case 30: /* type: CHAR_PTR  */
#line 958 "parser.y"
             { (yyval.node) = create_node("CHAR_PTR", 0); }
#line 2657 "parser.tab.c"
    break;

  case 31: /* block: BEGIN_T inner_block END_T  */
#line 962 "parser.y"
                              { (yyval.node) = (yyvsp[-1].node); }
#line 2663 "parser.tab.c"
    break;

  case 32: /* block: BEGIN_T END_T  */
#line 963 "parser.y"
                  { (yyval.node) = create_node("BLOCK", 0); }
#line 2669 "parser.tab.c"
    break;

  case 33: /* body: VAR optional_var_list block  */
#line 968 "parser.y"
                                { (yyval.node) = create_node("VAR_BLOCK", 2, (yyvsp[-1].node), (yyvsp[0].node)); }
#line 2675 "parser.tab.c"
    break;

  case 34: /* body: block  */
#line 969 "parser.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2681 "parser.tab.c"
    break;

  case 35: /* inner_block: nested_func inner_block  */
#line 973 "parser.y"
                            {
      if ((yyvsp[0].node) == NULL)
          (yyval.node) = create_node("BLOCK", 1, (yyvsp[-1].node));
      else {
          Node* merged = create_node("BLOCK", (yyvsp[0].node)->child_count + 1);
          merged->children[0] = (yyvsp[-1].node);
          for (int i = 0; i < (yyvsp[0].node)->child_count; i++)
              merged->children[i+1] = (yyvsp[0].node)->children[i];
          (yyval.node) = merged;
      }
  }
#line 2697 "parser.tab.c"
    break;

  case 36: /* inner_block: stmts inner_block  */
#line 984 "parser.y"
                      {
      if ((yyvsp[0].node) == NULL)
          (yyval.node) = create_node("BLOCK", 1, (yyvsp[-1].node));
      else {
          Node* merged = create_node("BLOCK", (yyvsp[0].node)->child_count + 1);
          merged->children[0] = (yyvsp[-1].node);
          for (int i = 0; i < (yyvsp[0].node)->child_count; i++)
              merged->children[i+1] = (yyvsp[0].node)->children[i];
          (yyval.node) = merged;
      }
  }
#line 2713 "parser.tab.c"
    break;

  case 37: /* inner_block: nested_func  */
#line 995 "parser.y"
                { (yyval.node) = create_node("BLOCK", 1, (yyvsp[0].node)); }
#line 2719 "parser.tab.c"
    break;

  case 38: /* inner_block: stmts  */
#line 996 "parser.y"
          { (yyval.node) = create_node("BLOCK", 1, (yyvsp[0].node)); }
#line 2725 "parser.tab.c"
    break;

  case 39: /* inner_block: %empty  */
#line 997 "parser.y"
                { (yyval.node) = create_node("BLOCK", 0); }
#line 2731 "parser.tab.c"
    break;

  case 40: /* stmts: stmt  */
#line 1001 "parser.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 2737 "parser.tab.c"
    break;

  case 41: /* stmts: stmt stmts  */
#line 1002 "parser.y"
               {
      if (strcmp((yyvsp[0].node)->name, "BLOCK") == 0) {
          Node* merged = create_node("BLOCK", (yyvsp[0].node)->child_count + 1);
          merged->children[0] = (yyvsp[-1].node);
          for (int i = 0; i < (yyvsp[0].node)->child_count; i++)
              merged->children[i+1] = (yyvsp[0].node)->children[i];
          (yyval.node) = merged;
      } else {
          (yyval.node) = create_node("BLOCK", 2, (yyvsp[-1].node), (yyvsp[0].node));
      }
  }
#line 2753 "parser.tab.c"
    break;

  case 42: /* stmt: RETURN expr ';'  */
#line 1016 "parser.y"
                    {
      current_func_has_return = 1;
      if (!check_return_type((yyvsp[-1].node))) {
          yyerror("Return type mismatch or returning from procedure");
      }
      (yyval.node) = create_node("RET", 1, (yyvsp[-1].node));
  }
#line 2765 "parser.tab.c"
    break;

  case 43: /* stmt: RETURN ';'  */
#line 1023 "parser.y"
               {
      current_func_has_return = 1;
      if (strlen(current_return_type) > 0)
          yyerror("Function with return type must return a value");
      (yyval.node) = create_node("RET", 0);
  }
#line 2776 "parser.tab.c"
    break;

  case 44: /* stmt: IDENTIFIER ASSIGN expr ';'  */
#line 1029 "parser.y"
                               {
    check_var_use((yyvsp[-3].str), yylineno);
    (yyval.node) = create_node("=", 2, create_node((yyvsp[-3].str), 0), (yyvsp[-1].node));
  }
#line 2785 "parser.tab.c"
    break;

  case 45: /* stmt: IDENTIFIER ASSIGN CALL IDENTIFIER '(' args ')' ';'  */
#line 1033 "parser.y"
                                                       {
    check_var_use((yyvsp[-7].str), yylineno);
    
    int func_found = check_function_call((yyvsp[-4].str), yylineno);
    
    // Count arguments
    int arg_count = 0;
    if (strcmp((yyvsp[-2].node)->name, "ARGS NONE") != 0) {
        arg_count = (yyvsp[-2].node)->child_count;
    }
    
    // Check parameter count even if function is not declared
    int required_count = get_function_param_count((yyvsp[-4].str));
    if (required_count >= 0 && required_count != arg_count) {
        param_count_mismatch = 1;
        sprintf(yytext, "%s", (yyvsp[-4].str));  // Store function name for error
        yylineno = (yylsp[-4]).first_line;   // Use the line number of function name token
        yyerror("Parameter count mismatch");
    } 
    else if (!func_found) {
        // Only trigger undeclared function error if param count was correct
        undeclared_function_error = 1;
        sprintf(yytext, "%s", (yyvsp[-4].str));
        yylineno = (yylsp[-4]).first_line;
        yyerror("Function called before declaration");
    }
    else {
        // If function exists and param count is correct, check param types
        check_param_types((yyvsp[-4].str), (yyvsp[-2].node), yylineno);
    }
    
    Node* call_node = create_node("CALL", 2, create_node((yyvsp[-4].str), 0), (yyvsp[-2].node));
    (yyval.node) = create_node("=", 2, create_node((yyvsp[-7].str), 0), call_node);
}
#line 2824 "parser.tab.c"
    break;

  case 46: /* stmt: CALL IDENTIFIER '(' args ')' ';'  */
#line 1067 "parser.y"
                                     {
    int func_found = check_function_call((yyvsp[-4].str), yylineno);
    
    // Count arguments
    int arg_count = 0;
    if (strcmp((yyvsp[-2].node)->name, "ARGS NONE") != 0) {
        arg_count = (yyvsp[-2].node)->child_count;
    }
    
    // Check parameter count even if function is not declared
    int required_count = get_function_param_count((yyvsp[-4].str));
    if (required_count >= 0 && required_count != arg_count) {
        param_count_mismatch = 1;
        sprintf(yytext, "%s", (yyvsp[-4].str));  // Store function name for error
        yylineno = (yylsp[-4]).first_line;  // Use the line number of function name token
        yyerror("Parameter count mismatch");
    } 
    else if (!func_found) {
        // Only trigger undeclared function error if param count was correct
        undeclared_function_error = 1;
        sprintf(yytext, "%s", (yyvsp[-4].str));
        yylineno = (yylsp[-4]).first_line;
        yyerror("Function called before declaration");
    }
    else {
        // If function exists and param count is correct, check param types
        check_param_types((yyvsp[-4].str), (yyvsp[-2].node), yylineno);
    }
    
    (yyval.node) = create_node("CALL", 2, create_node((yyvsp[-4].str), 0), (yyvsp[-2].node));
}
#line 2860 "parser.tab.c"
    break;

  case 47: /* stmt: IDENTIFIER '[' expr ']' ASSIGN expr ';'  */
#line 1098 "parser.y"
                                            {
    check_var_use((yyvsp[-6].str), yylineno);
    if (is_string_expr((yyvsp[-4].node))) {
        string_index_error = 1;
        yyerror("String expressions cannot be used as array indices");
    }
    else if (!is_int_expr((yyvsp[-4].node))) {
        array_index_error = 1;
        yyerror("Array index must be of integer type");
    }
    
    // Get variable type and debug it
    char* var_type = get_var_type((yyvsp[-6].str));
    printf("DEBUG: Assigning to array '%s' of type '%s'\n", 
           (yyvsp[-6].str), var_type ? var_type : "unknown");
    
    // Debug the expression being assigned
    debug_node((yyvsp[-1].node), "Expression being assigned");
    
    // Check if we're assigning a number or numeric array to a string array
    if (var_type && strcmp(var_type, "STRING") == 0) {
        // If it's an array element access
        if (strcmp((yyvsp[-1].node)->name, "ARRAY_ELEM") == 0) {
            char* src_array_name = (yyvsp[-1].node)->children[0]->name;
            char* src_array_type = get_var_type(src_array_name);
            
            printf("DEBUG: Source array '%s' of type '%s'\n", 
                   src_array_name, src_array_type ? src_array_type : "unknown");
            
            if (src_array_type && strcmp(src_array_type, "INT") == 0) {
                printf("ERROR: Cannot assign INT array element to STRING array\n");
                non_string_array_access_error = 1;
                yyerror("Cannot assign numeric array element to string array");
            }
        }
        // Direct numeric literal assignment
        else if ((yyvsp[-1].node)->child_count == 0 && isdigit((yyvsp[-1].node)->name[0])) {
            non_string_array_access_error = 1;
            yyerror("Cannot assign numeric value to string array element");
        }
    }
    
    Node* arr_elem = create_node("ARRAY_ELEM", 2, create_node((yyvsp[-6].str), 0), (yyvsp[-4].node));
    (yyval.node) = create_node("=", 2, arr_elem, (yyvsp[-1].node));
  }
#line 2910 "parser.tab.c"
    break;

  case 48: /* stmt: MUL IDENTIFIER ASSIGN expr ';'  */
#line 1143 "parser.y"
                                   {
    check_var_use((yyvsp[-3].str), yylineno);
    
    // Get variable type
    char* var_type = get_var_type((yyvsp[-3].str));
    
    // Check if it's a pointer type
    if (var_type) {
        if (strcmp(var_type, "INT_PTR") != 0 && 
            strcmp(var_type, "REAL_PTR") != 0 && 
            strcmp(var_type, "CHAR_PTR") != 0) {
            // Error: Can't dereference non-pointer
            dereference_type_error = 1;
            sprintf(yytext, "%s", (yyvsp[-3].str));
            yyerror("Cannot dereference non-pointer variable");
        }
    }
    
    (yyval.node) = create_node("= *", 2, create_node((yyvsp[-3].str), 0), (yyvsp[-1].node));
  }
#line 2935 "parser.tab.c"
    break;

  case 49: /* stmt: IF expr ':' body ELSE ':' body  */
#line 1163 "parser.y"
                                   {
      if (!is_bool_expr((yyvsp[-5].node))) {
          if_expr_type_error = 1;
          yyerror("If condition must be a boolean expression");
      }
      (yyval.node) = create_node("IF-ELSE", 3, (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[0].node));
  }
#line 2947 "parser.tab.c"
    break;

  case 50: /* stmt: IF expr ':' body ELIF expr ':' body ELSE ':' body  */
#line 1170 "parser.y"
                                                      {
      if (!is_bool_expr((yyvsp[-9].node)) || !is_bool_expr((yyvsp[-5].node))) {
          if_expr_type_error = 1;
          yyerror("If/elif condition must be a boolean expression");
      }
      (yyval.node) = create_node("IF-ELIF-ELSE", 5, (yyvsp[-9].node), (yyvsp[-7].node), (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[0].node));
  }
#line 2959 "parser.tab.c"
    break;

  case 51: /* stmt: IF expr ':' body ELIF expr ':' body  */
#line 1177 "parser.y"
                                        {
      if (!is_bool_expr((yyvsp[-6].node)) || !is_bool_expr((yyvsp[-2].node))) {
          if_expr_type_error = 1;
          yyerror("If/elif condition must be a boolean expression");
      }
      (yyval.node) = create_node("IF-ELIF", 4, (yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
  }
#line 2971 "parser.tab.c"
    break;

  case 52: /* stmt: IF expr ':' body  */
#line 1184 "parser.y"
                     {
      if (!is_bool_expr((yyvsp[-2].node))) {
          if_expr_type_error = 1;
          yyerror("If condition must be a boolean expression");
      }
      (yyval.node) = create_node("IF", 2, (yyvsp[-2].node), (yyvsp[0].node));
  }
#line 2983 "parser.tab.c"
    break;

  case 53: /* stmt: WHILE expr ':' body  */
#line 1191 "parser.y"
                        {
      if (!is_bool_expr((yyvsp[-2].node))) {
          if_expr_type_error = 1;
          yyerror("While condition must be a boolean expression");
      }
      (yyval.node) = create_node("WHILE", 2, (yyvsp[-2].node), (yyvsp[0].node));
  }
#line 2995 "parser.tab.c"
    break;

  case 54: /* stmt: DO ':' body WHILE expr ';'  */
#line 1198 "parser.y"
                               {
      if (!is_bool_expr((yyvsp[-1].node))) {
          if_expr_type_error = 1;
          yyerror("Do-while condition must be a boolean expression");
      }
      (yyval.node) = create_node("DO-WHILE", 2, (yyvsp[-3].node), (yyvsp[-1].node));
  }
#line 3007 "parser.tab.c"
    break;

  case 55: /* stmt: FOR '(' expr ';' expr ';' expr ')' ':' body  */
#line 1205 "parser.y"
                                                {
      if (!is_bool_expr((yyvsp[-5].node))) {
          if_expr_type_error = 1;
          yyerror("For condition must be a boolean expression");
      }
      Node* cond = create_node("FOR_COND", 3, (yyvsp[-7].node), (yyvsp[-5].node), (yyvsp[-3].node));
      (yyval.node) = create_node("FOR", 2, cond, (yyvsp[0].node));
  }
#line 3020 "parser.tab.c"
    break;

  case 56: /* stmt: VAR optional_var_list block  */
#line 1213 "parser.y"
                                {
      (yyval.node) = create_node("BODY", 2, (yyvsp[-1].node), (yyvsp[0].node));
  }
#line 3028 "parser.tab.c"
    break;

  case 57: /* stmt: block  */
#line 1216 "parser.y"
          {
      (yyval.node) = (yyvsp[0].node);
  }
#line 3036 "parser.tab.c"
    break;

  case 58: /* stmt: VAR optional_var_list stmt  */
#line 1219 "parser.y"
                               {
      (yyval.node) = create_node("VAR_BLOCK", 2, (yyvsp[-1].node), (yyvsp[0].node));
  }
#line 3044 "parser.tab.c"
    break;

  case 59: /* stmt: '(' IDENTIFIER ADD expr ')' '[' expr ']' ASSIGN expr ';'  */
#line 1222 "parser.y"
                                                             {
      check_var_use((yyvsp[-9].str), yylineno);
      array_pointer_arithmetic_error = 1;
      yyerror("Cannot add anything to array elements - they are not pointers");
      (yyval.node) = create_node("ERROR", 0);
  }
#line 3055 "parser.tab.c"
    break;

  case 60: /* stmt: '(' IDENTIFIER SUB expr ')' '[' expr ']' ASSIGN expr ';'  */
#line 1228 "parser.y"
                                                             {
      check_var_use((yyvsp[-9].str), yylineno);
      array_pointer_arithmetic_error = 1;
      yyerror("Cannot add anything to array elements - they are not pointers");
      (yyval.node) = create_node("ERROR", 0);
  }
#line 3066 "parser.tab.c"
    break;

  case 61: /* var_decls: VAR optional_var_list  */
#line 1238 "parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 3072 "parser.tab.c"
    break;

  case 62: /* var_decls: %empty  */
#line 1239 "parser.y"
                { (yyval.node) = create_node("BLOCK", 0); }
#line 3078 "parser.tab.c"
    break;

  case 63: /* optional_var_list: var_decl_list  */
#line 1243 "parser.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3084 "parser.tab.c"
    break;

  case 64: /* optional_var_list: %empty  */
#line 1244 "parser.y"
                { (yyval.node) = create_node("BLOCK", 0); }
#line 3090 "parser.tab.c"
    break;

  case 65: /* var_decl_list: var_decl_list var_single_decl  */
#line 1248 "parser.y"
                                  {
      Node* merged = create_node("BLOCK", (yyvsp[-1].node)->child_count + 1);
      for (int i = 0; i < (yyvsp[-1].node)->child_count; i++)
          merged->children[i] = (yyvsp[-1].node)->children[i];
      merged->children[(yyvsp[-1].node)->child_count] = (yyvsp[0].node);
      (yyval.node) = merged;
  }
#line 3102 "parser.tab.c"
    break;

  case 66: /* var_decl_list: var_single_decl  */
#line 1255 "parser.y"
                    { (yyval.node) = create_node("BLOCK", 1, (yyvsp[0].node)); }
#line 3108 "parser.tab.c"
    break;

  case 67: /* var_single_decl: TYPE BOOL ':' IDENTIFIER ',' IDENTIFIER ';'  */
#line 1259 "parser.y"
                                                {
      // Handle this specific case: type bool:c,d;
      add_var((yyvsp[-3].str), "BOOL");  // Add first variable with type
      add_var((yyvsp[-1].str), "BOOL");  // Add second variable with type
      
      // Create nodes for both variables
      char temp1[100], temp2[100];
      sprintf(temp1, "BOOL %s", (yyvsp[-3].str));
      sprintf(temp2, "BOOL %s", (yyvsp[-1].str));
      
      // Create a list of bool declarations
      Node* var_list = create_node("VAR_LIST", 2);
      var_list->children[0] = create_node(temp1, 0);
      var_list->children[1] = create_node(temp2, 0);
      
      (yyval.node) = var_list;
  }
#line 3130 "parser.tab.c"
    break;

  case 68: /* var_single_decl: TYPE type ':' id_list ';'  */
#line 1276 "parser.y"
                              {
      // Regular type declarations without initialization
      char* type_name = (yyvsp[-3].node)->name;
      
      if ((yyvsp[-1].node)->child_count == 0) {
          // Single variable declaration
          add_var((yyvsp[-1].node)->name, type_name);  // Add with type
          
          char temp[100];
          sprintf(temp, "%s %s", type_name, (yyvsp[-1].node)->name);
          (yyval.node) = create_node(temp, 0);
      } else {
          // Multiple variable declarations
          Node* var_list = create_node("VAR_LIST", (yyvsp[-1].node)->child_count);
          for (int i = 0; i < (yyvsp[-1].node)->child_count; i++) {
              add_var((yyvsp[-1].node)->children[i]->name, type_name);  // Add each with type
              
              char temp[100];
              sprintf(temp, "%s %s", type_name, (yyvsp[-1].node)->children[i]->name);
              var_list->children[i] = create_node(temp, 0);
          }
          (yyval.node) = var_list;
      }
  }
#line 3159 "parser.tab.c"
    break;

  case 69: /* var_single_decl: TYPE type ':' IDENTIFIER ':' expr ';'  */
#line 1300 "parser.y"
                                          {
      // Variable with initialization
      add_var((yyvsp[-3].str), (yyvsp[-5].node)->name);  // Add variable with type
      
      char temp[100];
      sprintf(temp, "%s %s", (yyvsp[-5].node)->name, (yyvsp[-3].str));
      Node* var_node = create_node(temp, 0);
      (yyval.node) = create_node("=", 2, var_node, (yyvsp[-1].node));
  }
#line 3173 "parser.tab.c"
    break;

  case 70: /* var_single_decl: TYPE BOOL ':' var_init_list ';'  */
#line 1309 "parser.y"
                                    {
      // This is handled in var_init_list
      (yyval.node) = (yyvsp[-1].node);
  }
#line 3182 "parser.tab.c"
    break;

  case 71: /* var_single_decl: TYPE BOOL ':' IDENTIFIER ';'  */
#line 1313 "parser.y"
                                 {  // Single boolean without initialization
      add_var((yyvsp[-1].str), "BOOL");  // Add with type
      
      char temp[100];
      sprintf(temp, "BOOL %s", (yyvsp[-1].str));
      (yyval.node) = create_node(temp, 0);
  }
#line 3194 "parser.tab.c"
    break;

  case 72: /* var_single_decl: TYPE type ':' string_decl_list ';'  */
#line 1320 "parser.y"
                                       {
    // Process each array declaration with the correct type
    for (int i = 0; i < (yyvsp[-1].node)->child_count; i++) {
        Node* array_node = (yyvsp[-1].node)->children[i];
        
        // Extract array name from the node name 
        if (strncmp(array_node->name, "STR ", 4) == 0) {
            char* name_start = array_node->name + 4;
            char* bracket_pos = strchr(name_start, '[');
            if (bracket_pos) {
                size_t name_len = bracket_pos - name_start;
                char* array_name = (char*)malloc(name_len + 1);
                strncpy(array_name, name_start, name_len);
                array_name[name_len] = '\0';
                
                printf("Adding array '%s' with type '%s'\n", array_name, (yyvsp[-3].node)->name);
                
                // Add the array with its correct type
                add_var(array_name, (yyvsp[-3].node)->name);
                free(array_name);
            }
        }
    }
    (yyval.node) = (yyvsp[-1].node);
}
#line 3224 "parser.tab.c"
    break;

  case 73: /* id_list: IDENTIFIER  */
#line 1348 "parser.y"
               {
      (yyval.node) = create_node((yyvsp[0].str), 0);  
  }
#line 3232 "parser.tab.c"
    break;

  case 74: /* id_list: IDENTIFIER ',' id_list  */
#line 1351 "parser.y"
                           {
      // Create a node for this identifier
      Node* id_node = create_node((yyvsp[-2].str), 0);
      
      // Create a list
      Node* merged;
      if ((yyvsp[0].node)->child_count == 0) {
          merged = create_node("ID_LIST", 2, id_node, (yyvsp[0].node));
      } else {
          merged = create_node("ID_LIST", (yyvsp[0].node)->child_count + 1);
          merged->children[0] = id_node;
          for (int i = 0; i < (yyvsp[0].node)->child_count; i++) {
              merged->children[i+1] = (yyvsp[0].node)->children[i];
          }
      }
      (yyval.node) = merged;
  }
#line 3254 "parser.tab.c"
    break;

  case 75: /* var_init_list: IDENTIFIER ':' expr  */
#line 1372 "parser.y"
                        {
      add_var((yyvsp[-2].str), "BOOL");  // Add boolean variable
      
      char temp[100];
      sprintf(temp, "BOOL %s", (yyvsp[-2].str));
      Node* var_node = create_node(temp, 0);
      (yyval.node) = create_node("=", 2, var_node, (yyvsp[0].node));
  }
#line 3267 "parser.tab.c"
    break;

  case 76: /* var_init_list: IDENTIFIER ':' expr ',' var_init_list  */
#line 1380 "parser.y"
                                          {
      add_var((yyvsp[-4].str), "BOOL");  // Add boolean variable
      
      char temp[100];
      sprintf(temp, "BOOL %s", (yyvsp[-4].str));
      Node* var_node = create_node(temp, 0);
      Node* assign_node = create_node("=", 2, var_node, (yyvsp[-2].node));
      Node* merged = create_node("BLOCK", (yyvsp[0].node)->child_count + 1);
      merged->children[0] = assign_node;
      for (int i = 0; i < (yyvsp[0].node)->child_count; i++)
          merged->children[i+1] = (yyvsp[0].node)->children[i];
      (yyval.node) = merged;
  }
#line 3285 "parser.tab.c"
    break;

  case 77: /* string_decl_list: string_decl_list ',' string_decl  */
#line 1397 "parser.y"
                                     {
      Node* merged = create_node("BLOCK", (yyvsp[-2].node)->child_count + 1);
      for (int i = 0; i < (yyvsp[-2].node)->child_count; i++)
          merged->children[i] = (yyvsp[-2].node)->children[i];
      merged->children[(yyvsp[-2].node)->child_count] = (yyvsp[0].node);
      (yyval.node) = merged;
  }
#line 3297 "parser.tab.c"
    break;

  case 78: /* string_decl_list: string_decl  */
#line 1404 "parser.y"
                {
      (yyval.node) = create_node("BLOCK", 1, (yyvsp[0].node));
  }
#line 3305 "parser.tab.c"
    break;

  case 79: /* string_decl: IDENTIFIER '[' INT_LITERAL ']'  */
#line 1410 "parser.y"
                                   {
      char temp[100];
      sprintf(temp, "STR %s[%s]", (yyvsp[-3].str), (yyvsp[-1].str));
      (yyval.node) = create_node(temp, 0);
      // Do NOT add the variable here - it will be added in the parent rule
  }
#line 3316 "parser.tab.c"
    break;

  case 80: /* string_decl: IDENTIFIER '[' INT_LITERAL ']' ':' STRING_LITERAL  */
#line 1416 "parser.y"
                                                      {
      char temp[100];
      sprintf(temp, "STR %s[%s]:%s", (yyvsp[-5].str), (yyvsp[-3].str), (yyvsp[0].str));
      (yyval.node) = create_node(temp, 0);
      // Do NOT add the variable here - it will be added in the parent rule
  }
#line 3327 "parser.tab.c"
    break;

  case 81: /* expr: expr ADD expr  */
#line 1426 "parser.y"
                  { 
    Node* node = create_node("+", 2, (yyvsp[-2].node), (yyvsp[0].node));
    
    // Type checking
    char* left_type = get_expr_type((yyvsp[-2].node));
    char* right_type = get_expr_type((yyvsp[0].node));
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            arith_type_error = 1;
            yyerror("Arithmetic operators require numeric operands");
        }
    }
    
    (yyval.node) = node;
  }
#line 3350 "parser.tab.c"
    break;

  case 82: /* expr: expr SUB expr  */
#line 1444 "parser.y"
                  { 
    Node* node = create_node("-", 2, (yyvsp[-2].node), (yyvsp[0].node));
    
    // Type checking
    char* left_type = get_expr_type((yyvsp[-2].node));
    char* right_type = get_expr_type((yyvsp[0].node));
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            arith_type_error = 1;
            yyerror("Arithmetic operators require numeric operands");
        }
    }
    
    (yyval.node) = node;
  }
#line 3373 "parser.tab.c"
    break;

  case 83: /* expr: expr MUL expr  */
#line 1462 "parser.y"
                  { 
    Node* node = create_node("*", 2, (yyvsp[-2].node), (yyvsp[0].node));
    
    // Type checking
    char* left_type = get_expr_type((yyvsp[-2].node));
    char* right_type = get_expr_type((yyvsp[0].node));
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            arith_type_error = 1;
            yyerror("Arithmetic operators require numeric operands");
        }
    }
    
    (yyval.node) = node;
  }
#line 3396 "parser.tab.c"
    break;

  case 84: /* expr: expr DIV expr  */
#line 1480 "parser.y"
                  { 
    Node* node = create_node("/", 2, (yyvsp[-2].node), (yyvsp[0].node));
    
    // Type checking
    char* left_type = get_expr_type((yyvsp[-2].node));
    char* right_type = get_expr_type((yyvsp[0].node));
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            arith_type_error = 1;
            yyerror("Arithmetic operators require numeric operands");
        }
    }
    
    (yyval.node) = node;
  }
#line 3419 "parser.tab.c"
    break;

  case 85: /* expr: expr EQ expr  */
#line 1498 "parser.y"
                 { 
    Node* node = create_node("==", 2, (yyvsp[-2].node), (yyvsp[0].node));
    
    // Type checking
    char* left_type = get_expr_type((yyvsp[-2].node));
    char* right_type = get_expr_type((yyvsp[0].node));
    
    if (left_type && right_type) {
        // Types must be compatible
        if (strcmp(left_type, right_type) != 0 &&
            !((strcmp(left_type, "INT") == 0 && strcmp(right_type, "REAL") == 0) ||
              (strcmp(left_type, "REAL") == 0 && strcmp(right_type, "INT") == 0))) {
            equality_type_error = 1;
            yyerror("Equality operators require compatible types");
        }
    }
    
    (yyval.node) = node;
  }
#line 3443 "parser.tab.c"
    break;

  case 86: /* expr: expr NE expr  */
#line 1517 "parser.y"
                 { 
    Node* node = create_node("!=", 2, (yyvsp[-2].node), (yyvsp[0].node));
    
    // Type checking
    char* left_type = get_expr_type((yyvsp[-2].node));
    char* right_type = get_expr_type((yyvsp[0].node));
    
    if (left_type && right_type) {
        // Types must be compatible
        if (strcmp(left_type, right_type) != 0 &&
            !((strcmp(left_type, "INT") == 0 && strcmp(right_type, "REAL") == 0) ||
              (strcmp(left_type, "REAL") == 0 && strcmp(right_type, "INT") == 0))) {
            equality_type_error = 1;
            yyerror("Equality operators require compatible types");
        }
    }
    
    (yyval.node) = node;
  }
#line 3467 "parser.tab.c"
    break;

  case 87: /* expr: expr GT expr  */
#line 1536 "parser.y"
                 { 
    Node* node = create_node(">", 2, (yyvsp[-2].node), (yyvsp[0].node));
    
    // Type checking
    char* left_type = get_expr_type((yyvsp[-2].node));
    char* right_type = get_expr_type((yyvsp[0].node));
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            comparison_type_error = 1;
            yyerror("Comparison operators require numeric operands");
        }
    }
    
    (yyval.node) = node;
  }
#line 3490 "parser.tab.c"
    break;

  case 88: /* expr: expr GE expr  */
#line 1554 "parser.y"
                 { 
    Node* node = create_node(">=", 2, (yyvsp[-2].node), (yyvsp[0].node));
    
    // Type checking
    char* left_type = get_expr_type((yyvsp[-2].node));
    char* right_type = get_expr_type((yyvsp[0].node));
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            comparison_type_error = 1;
            yyerror("Comparison operators require numeric operands");
        }
    }
    
    (yyval.node) = node;
  }
#line 3513 "parser.tab.c"
    break;

  case 89: /* expr: expr LT expr  */
#line 1572 "parser.y"
                 { 
    Node* node = create_node("<", 2, (yyvsp[-2].node), (yyvsp[0].node));
    
    // Type checking
    char* left_type = get_expr_type((yyvsp[-2].node));
    char* right_type = get_expr_type((yyvsp[0].node));
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            comparison_type_error = 1;
            yyerror("Comparison operators require numeric operands");
        }
    }
    
    (yyval.node) = node;
  }
#line 3536 "parser.tab.c"
    break;

  case 90: /* expr: expr LE expr  */
#line 1590 "parser.y"
                 { 
    Node* node = create_node("<=", 2, (yyvsp[-2].node), (yyvsp[0].node));
    
    // Type checking
    char* left_type = get_expr_type((yyvsp[-2].node));
    char* right_type = get_expr_type((yyvsp[0].node));
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            comparison_type_error = 1;
            yyerror("Comparison operators require numeric operands");
        }
    }
    
    (yyval.node) = node;
  }
#line 3559 "parser.tab.c"
    break;

  case 91: /* expr: expr AND expr  */
#line 1609 "parser.y"
                  { 
      Node* node = create_node("AND", 2, (yyvsp[-2].node), (yyvsp[0].node));
      // Type checking...
      char* left_type = get_expr_type((yyvsp[-2].node));
      char* right_type = get_expr_type((yyvsp[0].node));
      
      if (left_type && right_type) {
          if (strcmp(left_type, "BOOL") != 0 || strcmp(right_type, "BOOL") != 0) {
              logical_op_type_error = 1;
              yyerror("Logical operators require boolean operands");
          }
      }
      (yyval.node) = node;
  }
#line 3578 "parser.tab.c"
    break;

  case 92: /* expr: expr BITWISE_AND expr  */
#line 1623 "parser.y"
                          { 
      Node* node = create_node("BITWISE_AND", 2, (yyvsp[-2].node), (yyvsp[0].node));
      // Type checking...
      char* left_type = get_expr_type((yyvsp[-2].node));
      char* right_type = get_expr_type((yyvsp[0].node));
      
      if (left_type && right_type) {
          if (strcmp(left_type, "BOOL") != 0 || strcmp(right_type, "BOOL") != 0) {
              logical_op_type_error = 1;
              yyerror("Logical && operator requires boolean operands");
          }
      }
      (yyval.node) = node;
  }
#line 3597 "parser.tab.c"
    break;

  case 93: /* expr: expr OR expr  */
#line 1637 "parser.y"
                 { 
      Node* node = create_node("OR", 2, (yyvsp[-2].node), (yyvsp[0].node));
      // Type checking...
      char* left_type = get_expr_type((yyvsp[-2].node));
      char* right_type = get_expr_type((yyvsp[0].node));
      
      if (left_type && right_type) {
          if (strcmp(left_type, "BOOL") != 0 || strcmp(right_type, "BOOL") != 0) {
              logical_op_type_error = 1;
              yyerror("Logical operators require boolean operands");
          }
      }
      (yyval.node) = node;
  }
#line 3616 "parser.tab.c"
    break;

  case 94: /* expr: expr BITWISE_OR expr  */
#line 1651 "parser.y"
                         {
      Node* node = create_node("BITWISE_OR", 2, (yyvsp[-2].node), (yyvsp[0].node));
      // Type checking...
      char* left_type = get_expr_type((yyvsp[-2].node));
      char* right_type = get_expr_type((yyvsp[0].node));
      
      if (left_type && right_type) {
          if (strcmp(left_type, "BOOL") != 0 || strcmp(right_type, "BOOL") != 0) {
              logical_op_type_error = 1;
              yyerror("Logical || operator requires boolean operands");
          }
      }
      (yyval.node) = node;
  }
#line 3635 "parser.tab.c"
    break;

  case 95: /* expr: NOT expr  */
#line 1665 "parser.y"
             { 
      Node* node = create_node("NOT", 1, (yyvsp[0].node));
      // Type checking...
      char* operand_type = get_expr_type((yyvsp[0].node));
      
      if (operand_type) {
          if (strcmp(operand_type, "BOOL") != 0) {
              logical_op_type_error = 1;
              yyerror("NOT operator requires a boolean operand");
          }
      }
      (yyval.node) = node;
  }
#line 3653 "parser.tab.c"
    break;

  case 96: /* expr: BITWISE_NOT expr  */
#line 1678 "parser.y"
                     { 
      Node* node = create_node("BITWISE_NOT", 1, (yyvsp[0].node));
      
      // Type checking - operand must be boolean
      char* operand_type = get_expr_type((yyvsp[0].node));
      
      if (operand_type) {
          if (strcmp(operand_type, "BOOL") != 0) {
              logical_op_type_error = 1;
              yyerror("Bitwise NOT operator requires a boolean operand");
          }
      }
      
      (yyval.node) = node;
  }
#line 3673 "parser.tab.c"
    break;

  case 97: /* expr: '(' expr ')'  */
#line 1694 "parser.y"
                 { (yyval.node) = (yyvsp[-1].node); }
#line 3679 "parser.tab.c"
    break;

  case 98: /* expr: SUB expr  */
#line 1695 "parser.y"
                          { (yyval.node) = create_node("-", 2, create_node("0", 0), (yyvsp[0].node)); }
#line 3685 "parser.tab.c"
    break;

  case 99: /* expr: MUL IDENTIFIER  */
#line 1696 "parser.y"
                   { 
    check_var_use((yyvsp[0].str), yylineno);
    
    // Get variable type
    char* var_type = get_var_type((yyvsp[0].str));
    
    // Check if it's a pointer type
    if (var_type) {
        if (strcmp(var_type, "INT_PTR") != 0 && 
            strcmp(var_type, "REAL_PTR") != 0 && 
            strcmp(var_type, "CHAR_PTR") != 0) {
            // Error: Can't dereference non-pointer
            dereference_type_error = 1;
            sprintf(yytext, "%s", (yyvsp[0].str));
            yyerror("Cannot dereference non-pointer variable");
        }
    }
    
    (yyval.node) = create_node("*", 1, create_node((yyvsp[0].str), 0)); 
  }
#line 3710 "parser.tab.c"
    break;

  case 100: /* expr: '(' IDENTIFIER ADD expr ')' '[' expr ']'  */
#line 1717 "parser.y"
                                             { 
      check_var_use((yyvsp[-6].str), yylineno);
      array_pointer_arithmetic_error = 1;
      yylineno = (yylsp[-2]).first_line;  // Line of the '[' token
      yyerror("Cannot add anything to array elements - they are not pointers");
      (yyval.node) = create_node("ERROR", 0);
  }
#line 3722 "parser.tab.c"
    break;

  case 101: /* expr: '(' IDENTIFIER SUB expr ')' '[' expr ']'  */
#line 1724 "parser.y"
                                             { 
      check_var_use((yyvsp[-6].str), yylineno);
      array_pointer_arithmetic_error = 1;
      yyerror("Cannot add anything to array elements - they are not pointers");
      (yyval.node) = create_node("ERROR", 0);
  }
#line 3733 "parser.tab.c"
    break;

  case 102: /* expr: ADDR IDENTIFIER  */
#line 1730 "parser.y"
                    { 
    check_var_use((yyvsp[0].str), yylineno);
    
    // Get the variable type
    char* var_type = get_var_type((yyvsp[0].str));
    
    // Check if it's a valid type for address-of
    if (var_type) {
        if (strcmp(var_type, "INT") != 0 && 
            strcmp(var_type, "REAL") != 0 && 
            strcmp(var_type, "CHAR") != 0 && 
            strcmp(var_type, "STRING") != 0) {
            // Error: Can't take address of this type
            printf("Semantic error at line %d: Cannot take address of variable of type '%s'\n", 
                   yylineno, var_type);
            yytext = (yyvsp[0].str); // Store the variable name for error message
            yyerror("Cannot take address of this type");
        }
    }
    
    (yyval.node) = create_node("&", 1, create_node((yyvsp[0].str), 0)); 
  }
#line 3760 "parser.tab.c"
    break;

  case 103: /* expr: ADDR IDENTIFIER '[' expr ']'  */
#line 1752 "parser.y"
                                 { 
    check_var_use((yyvsp[-3].str), yylineno);
    
    // Check if index is valid (integer)
    if (!is_int_expr((yyvsp[-1].node))) {
        array_index_error = 1;
        yyerror("Array index must be of integer type");
    }
    
    // Create array element node
    Node* arr_elem = create_node("ARRAY_ELEM", 2, create_node((yyvsp[-3].str), 0), (yyvsp[-1].node));
    
    // Create address-of node
    (yyval.node) = create_node("&", 1, arr_elem); 
  }
#line 3780 "parser.tab.c"
    break;

  case 104: /* expr: ADDR '(' expr ')'  */
#line 1767 "parser.y"
                      { 
    // This should always produce an error
    yyerror("Cannot take address of an expression");
    (yyval.node) = create_node("ERROR", 0);
  }
#line 3790 "parser.tab.c"
    break;

  case 105: /* expr: MUL '(' expr ')'  */
#line 1772 "parser.y"
                     { 
    // Type checking - operand must be a pointer
    char* expr_type = get_expr_type((yyvsp[-1].node));
    
    if (expr_type) {
        if (strcmp(expr_type, "INT_PTR") != 0 && 
            strcmp(expr_type, "REAL_PTR") != 0 && 
            strcmp(expr_type, "CHAR_PTR") != 0) {
            // Error: Can't dereference non-pointer
            dereference_type_error = 1;
            yyerror("Cannot dereference non-pointer expression");
        }
    }
    
    (yyval.node) = create_node("*", 1, (yyvsp[-1].node)); 
  }
#line 3811 "parser.tab.c"
    break;

  case 106: /* expr: IDENTIFIER  */
#line 1788 "parser.y"
               { 
    check_var_use((yyvsp[0].str), yylineno);
    (yyval.node) = create_node((yyvsp[0].str), 0); 
  }
#line 3820 "parser.tab.c"
    break;

  case 107: /* expr: IDENTIFIER '(' args ')'  */
#line 1792 "parser.y"
                            { 
    int func_found = check_function_call((yyvsp[-3].str), yylineno);
    
    // Count arguments
    int arg_count = 0;
    if (strcmp((yyvsp[-1].node)->name, "ARGS NONE") != 0) {
        arg_count = (yyvsp[-1].node)->child_count;
    }
    
    // Check parameter count even if function is not declared
    int required_count = get_function_param_count((yyvsp[-3].str));
    if (required_count >= 0 && required_count != arg_count) {
        param_count_mismatch = 1;
        sprintf(yytext, "%s", (yyvsp[-3].str));  // Store function name for error
        yylineno = (yylsp[-3]).first_line;   // Use the line number of function name token
        yyerror("Parameter count mismatch");
    } 
    else if (!func_found) {
        // Only trigger undeclared function error if param count was correct
        undeclared_function_error = 1;
        sprintf(yytext, "%s", (yyvsp[-3].str));
        yylineno = (yylsp[-3]).first_line;
        yyerror("Function called before declaration");
    }
    else {
        // If function exists and param count is correct, check param types
        check_param_types((yyvsp[-3].str), (yyvsp[-1].node), yylineno);
    }
    
    (yyval.node) = create_node("CALL", 2, create_node((yyvsp[-3].str), 0), (yyvsp[-1].node)); 
  }
#line 3856 "parser.tab.c"
    break;

  case 108: /* expr: IDENTIFIER '[' expr ']'  */
#line 1823 "parser.y"
                            { 
    check_var_use((yyvsp[-3].str), yylineno);
    
    // Check if the index expression is a REAL literal or identifier
    if ((yyvsp[-1].node)->child_count == 0) {
        // Check if it's a REAL literal
        if (strchr((yyvsp[-1].node)->name, '.')) {
            array_index_error = 1;
            yyerror("Array index must be of integer type");
        }
        
        // Check if it's a variable of type REAL
        char* var_type = get_var_type((yyvsp[-1].node)->name);
        if (var_type && strcmp(var_type, "REAL") == 0) {
            array_index_error = 1;
            yyerror("Array index must be of integer type");
        }
    }
    
    // String expressions as array indices
    if (is_string_expr((yyvsp[-1].node))) {
        string_index_error = 1;
        yyerror("String expressions cannot be used as array indices");
    }
    
    (yyval.node) = create_node("ARRAY_ELEM", 2, create_node((yyvsp[-3].str), 0), (yyvsp[-1].node)); 
  }
#line 3888 "parser.tab.c"
    break;

  case 109: /* expr: IDENTIFIER  */
#line 1850 "parser.y"
               { 
    check_var_use((yyvsp[0].str), yylineno);
    (yyval.node) = create_node((yyvsp[0].str), 0); 
  }
#line 3897 "parser.tab.c"
    break;

  case 110: /* expr: INT_LITERAL  */
#line 1854 "parser.y"
                { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 3903 "parser.tab.c"
    break;

  case 111: /* expr: REAL_LITERAL  */
#line 1855 "parser.y"
                 { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 3909 "parser.tab.c"
    break;

  case 112: /* expr: CHAR_LITERAL  */
#line 1856 "parser.y"
                 { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 3915 "parser.tab.c"
    break;

  case 113: /* expr: STRING_LITERAL  */
#line 1857 "parser.y"
                   { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 3921 "parser.tab.c"
    break;

  case 114: /* expr: TRUE  */
#line 1858 "parser.y"
         { (yyval.node) = create_node("TRUE", 0); }
#line 3927 "parser.tab.c"
    break;

  case 115: /* expr: FALSE  */
#line 1859 "parser.y"
          { (yyval.node) = create_node("FALSE", 0); }
#line 3933 "parser.tab.c"
    break;

  case 116: /* expr: NULL_T  */
#line 1860 "parser.y"
           { (yyval.node) = create_node("NULL", 0); }
#line 3939 "parser.tab.c"
    break;

  case 117: /* expr: PIPE_SYMBOL IDENTIFIER PIPE_SYMBOL  */
#line 1861 "parser.y"
                                       { 
	    check_var_use((yyvsp[-1].str), yylineno);
	    
	    // Type checking - the operand must be a string
	    char* var_type = get_var_type((yyvsp[-1].str));
	    if (var_type && strcmp(var_type, "STRING") != 0) {
		length_type_error = 1;
		yyerror("Length operator requires a string operand");
	    }
	    
	    (yyval.node) = create_node("LEN", 1, create_node((yyvsp[-1].str), 0)); 
  }
#line 3956 "parser.tab.c"
    break;

  case 118: /* expr: '(' expr ADD expr ')' '[' expr ']'  */
#line 1873 "parser.y"
                                       {
    array_pointer_arithmetic_error = 1;
    yyerror("Cannot add anything to array elements - they are not pointers");
    (yyval.node) = create_node("ARRAY_ELEM", 3, (yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-1].node));
  }
#line 3966 "parser.tab.c"
    break;

  case 119: /* args: expr  */
#line 1882 "parser.y"
         { (yyval.node) = create_node("ARGS", 1, (yyvsp[0].node)); }
#line 3972 "parser.tab.c"
    break;

  case 120: /* args: expr ',' args  */
#line 1883 "parser.y"
                  {
      if (strcmp((yyvsp[0].node)->name, "ARGS") == 0) {
          Node* merged = create_node("ARGS", (yyvsp[0].node)->child_count + 1);
          merged->children[0] = (yyvsp[-2].node);
          for (int i = 0; i < (yyvsp[0].node)->child_count; i++)
              merged->children[i+1] = (yyvsp[0].node)->children[i];
          (yyval.node) = merged;
      } else {
          (yyval.node) = create_node("ARGS", 2, (yyvsp[-2].node), (yyvsp[0].node));
      }
  }
#line 3988 "parser.tab.c"
    break;

  case 121: /* args: %empty  */
#line 1894 "parser.y"
                { (yyval.node) = create_node("ARGS NONE", 0); }
#line 3994 "parser.tab.c"
    break;


#line 3998 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1897 "parser.y"


/* הדפסת שגיאות יפות */
void yyerror(const char* s) {
    if (comma_error) {
        printf("Syntax error at line %d: parameters must be separated by semicolon\n", yylineno);
        comma_error = 0;
    }
    else if (param_type_mismatch) {
    printf("Semantic error at line %d: Function '%s' has parameter type mismatch\n", yylineno, yytext);
    param_type_mismatch = 0;
    }/*
    else if (param_order_error) {
        printf("Semantic error at line %d: Parameters must be ordered as par1, par2, etc.\n", yylineno);
        param_order_error = 0;
    }*/
    else if (param_error) {
        printf("Syntax error at line %d: no type defined\n", yylineno);
        param_error = 0;
    }
    
    
    else if (return_type_error) {
        printf("Semantic error at line %d: Return type mismatch\n", yylineno);
        return_type_error = 0;
    }
    else if (missing_return_error) {
        printf("Semantic error at line %d: Function with return type must end with a return statement\n", yylineno);
        missing_return_error = 0;
    }
    else if (proc_return_error) {
        printf("Semantic error at line %d: Cannot return a value from a procedure\n", yylineno);
        proc_return_error = 0;
    }
    else if (multiple_main_error) {
        printf("Semantic error at line %d: Program cannot have multiple _main_() procedures\n", yylineno);
        multiple_main_error = 0;
    }
    else if (main_with_params_error) {
        printf("Semantic error at line %d: _main_() cannot have parameters\n", yylineno);
        main_with_params_error = 0;
    }
    else if (duplicate_function_error) {
    printf("Semantic error at line %d: Function with name '%s' already defined\n", yylineno, yytext);
    duplicate_function_error = 0;
    }
    else if (duplicate_var_error) {
    printf("Semantic error at line %d: Variable already defined in this scope\n", yylineno);
    duplicate_var_error = 0;
    }
    else if (undeclared_function_error) {
    printf("Semantic error at line %d: Function '%s' called before declaration\n", yylineno, yytext);
    undeclared_function_error = 0;
    }
    else if (undeclared_var_error) {
    printf("Semantic error at line %d: Variable '%s' used before declaration\n", yylineno, yytext);
    undeclared_var_error = 0;
    }
    else if (param_count_mismatch) {
    int required = get_function_param_count(yytext);
    printf("Semantic error at line %d: Function '%s' requires %d parameters but was called with a different number\n", yylineno, yytext, required);
    param_count_mismatch = 0;
    }
    else if (string_return_error) {
    printf("Semantic error at line %d: Functions cannot return type string\n", yylineno);
    string_return_error = 0;
    }
    else if (if_expr_type_error) {
    printf("Semantic error at line %d: Conditional expression must be of boolean type\n", yylineno);
    if_expr_type_error = 0;
    }
    else if (array_index_error) {
    printf("Semantic error at line %d: Array index must be of integer type\n", yylineno);
    array_index_error = 0;
    }
    else if (string_index_error) {
    printf("Semantic error at line %d: String expressions cannot be used as array indices\n", yylineno);
    string_index_error = 0;
    }
    else if (non_string_array_access_error) {
        printf("Semantic error at line %d: Type mismatch in array element assignment\n", yylineno);
        non_string_array_access_error = 0;
    }
    else if (array_pointer_arithmetic_error) {
        printf("Semantic error at line %d: Cannot add anything to array elements - they are not pointers\n",   yylineno);
        array_pointer_arithmetic_error = 0;
    }
    else if (arith_type_error) {
        printf("Semantic error at line %d: Arithmetic operators require numeric operands\n", yylineno);
        arith_type_error = 0;
    }
    else if (logical_type_error) {
        printf("Semantic error at line %d: Logical operators require boolean operands\n", yylineno);
        logical_type_error = 0;
    }
    else if (logical_op_type_error) {
        printf("Semantic error at line %d: Logical operators require boolean operands\n", yylineno);
        logical_op_type_error = 0;
    }
    else if (comparison_type_error) {
	    printf("Semantic error at line %d: Comparison operators require numeric operands\n", yylineno);
	    comparison_type_error = 0;
    }
 
    else if (equality_type_error) {
        printf("Semantic error at line %d: Equality operators require compatible types\n", yylineno);
        equality_type_error = 0;
    }
    else if (length_type_error) {
        printf("Semantic error at line %d: Length operator requires a string operand\n", yylineno);
        length_type_error = 0;
    }
    else if (logical_op_type_error) {
        printf("Semantic error at line %d: Logical operators require boolean operands\n", yylineno);
        logical_op_type_error = 0;
    }
    else if (invalid_address_error) {
    printf("Semantic error at line %d: Cannot take address of %s\n", yylineno, yytext);
    invalid_address_error = 0;
    }
    else if (dereference_type_error) {
    printf("Semantic error at line %d: Cannot dereference non-pointer variable '%s'\n", yylineno, yytext);
    dereference_type_error = 0;
    }
    else {
        printf("Syntax error at line %d: %s\n", yylineno, s);
    }
    printf("Debug: Error at token '%s'\n", yytext);
    exit(1);
}

int main() {
    if (yyparse() == 0) {
        if (!has_main) {
            fprintf(stderr, "Error: Program must have exactly one _main_() procedure\n");
            return 1;
        }

        printf("Parsing completed successfully.\n");
        
        // Print AST (optional, remove if you don't want it)
        printf("AST:\n");
        print_ast(root, 0);
        
        // Generate 3AC
        printf("\n3AC Code:\n");
        gen_code(root);
        
        return 0;
    }
    return 1;
}
