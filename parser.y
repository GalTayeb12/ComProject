%{
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



void add_function(char* name, int param_count, Node* params, char* ret_type) {
    printf("DEBUG: Registering function '%s' with %d parameters, return type: '%s'\n", 
           name, param_count, ret_type ? ret_type : "none");
    if (is_function_defined(name) != 0) {
        duplicate_function_error = 1;
        yyerror("Error: Function already defined");
        return;
    }
    
    if (function_count < MAX_FUNCTIONS) {
        function_table[function_count].name = strdup(name);
        function_table[function_count].param_count = param_count;
        
        // Store return type
        if (ret_type && strlen(ret_type) > 0) {
            function_table[function_count].return_type = strdup(ret_type);
        } else {
            function_table[function_count].return_type = NULL; // Procedure
        }
        
        // Allocate and store parameter types (existing code)
        if (param_count > 0) {
            function_table[function_count].param_types = (char**)malloc(sizeof(char*) * param_count);
            
            for (int i = 0; i < param_count; i++) {
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

int is_procedure(const char* func_name) {
    for (int i = 0; i < function_count; i++) {
        if (strcmp(function_table[i].name, func_name) == 0) {
            // Return 1 if return_type is NULL or empty (procedure)
            return (function_table[i].return_type == NULL || 
                    strlen(function_table[i].return_type) == 0);
        }
    }
    return 1; // If function not found, assume it's a procedure to be safe
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
%}

%union {
    char* str;
    Node* node;
}

%token DEF RETURNS RETURN IF ELSE ELIF WHILE FOR DO VAR BEGIN_T END_T CALL NULL_T TRUE FALSE TYPE
%token INT REAL CHAR BOOL STRING INT_PTR REAL_PTR CHAR_PTR
%token AND OR NOT
%token EQ NE LE GE LT GT ASSIGN
%token ADD SUB MUL DIV ADDR DEREF PIPE_SYMBOL
%token <str> IDENTIFIER INT_LITERAL REAL_LITERAL CHAR_LITERAL STRING_LITERAL
%token BITWISE_OR
%token BITWISE_AND
%token BITWISE_NOT

%type <node> 
    program funcs func nested_func func_header 
    parameters parameter ret_type type
    block inner_block stmts stmt expr args
    var_decls optional_var_list var_decl_list var_single_decl var_init_list string_decl_list string_decl body
    id_list

// קדימויות ואסוציאטיביות

%left OR BITWISE_OR
%left AND BITWISE_AND
%left EQ NE
%left LT LE GT GE
%left ADD SUB
%left MUL DIV
%right NOT ADDR
%right UMINUS
%right BITWISE_NOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE ELIF

%%

program
  : funcs { root = create_node("CODE", 1, $1); }
;

funcs
  : func { $$ = $1; }
  | func funcs { $$ = create_node("FUNC", 2, $1, $2); }
  | /* empty */ { $$ = NULL; }
;

func
  : func_header var_decls block {
      if (!ends_with_return($3) && strlen(current_return_type) > 0) {
          missing_return_error = 1;
          yyerror("Function with return type must end with a return statement");
      }
      $$ = create_node($1->name, 3, $1->children[0], $1->children[1], create_node("BODY", 2, $2, $3));
      pop_return_type();
      clear_vars(); // Clear variables after the function is fully processed
  }
;

nested_func
  : func_header var_decls block {
      if (!ends_with_return($3) && strlen(current_return_type) > 0) {
          missing_return_error = 1;
          yyerror("Function with return type must end with a return statement");
      }
      $$ = create_node($1->name, 3, $1->children[0], $1->children[1], create_node("BODY", 2, $2, $3));
      pop_return_type();
      clear_vars(); // Clear variables after the function is fully processed
  }
;

func_header
  : DEF IDENTIFIER '(' parameters ')' ':' RETURNS ret_type {
      param_count = 0;
      push_return_type(return_type_to_push);
      Node* params = $4;
      Node* ret_type_node = $8;
      if (strcmp($2, "_main_") == 0) {
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
      add_function($2, param_count, params, $8->name); // Pass return type
      $$ = create_node($2, 2, params, ret_type_node);
  }
  | DEF IDENTIFIER '(' parameters ')' ':' {
      param_count = 0;
      push_return_type(""); 
      Node* params = $4;
      Node* ret_type_node = create_node("RET NONE", 0);
      
      // Check if this is the main function
      if (strcmp($2, "_main_") == 0) {
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
      
      add_function($2, param_count, params, NULL); // NULL for procedures
      $$ = create_node($2, 2, params, ret_type_node);
  }
;

parameters
  : parameter ';' parameters {
      Node* pars = create_node("PARS", $3->child_count + 1);
      pars->children[0] = $1;
      
      // Check if parameter is in correct order
      char* param_str = $1->name;
      int param_index = 1; // First parameter should be par1
      /*
      if (!check_param_order(param_str, param_index)) {
          // Just set the flag, don't exit
          param_order_error = 1;
      }*/
      
      for (int i = 0; i < $3->child_count; i++) {
          pars->children[i+1] = $3->children[i];
          
          // Check each parameter in the list
          param_str = $3->children[i]->name;
          param_index = i + 2; // Next parameters should be par2, par3, etc.
          /*
          if (!check_param_order(param_str, param_index)) {
              // Just set the flag, don't exit
              param_order_error = 1;
          }*/
      }
      $$ = pars;
  }
  | parameter {
      // Check if this single parameter is par1
      char* param_str = $1->name;/*
      if (!check_param_order(param_str, 1)) {
          // Just set the flag, don't exit
          param_order_error = 1;
      }*/
      $$ = create_node("PARS", 1, $1);
  }
  | /* empty */ {
      $$ = create_node("PARS NONE", 0);
  }
;

parameter
  : IDENTIFIER type ':' IDENTIFIER {
      char temp[100];
      sprintf(temp, "par%s %s %s", $1 + 3, $2->name, $4);
      $$ = create_node(temp, 0);
      // We'll add the variable when processing all parameters in func_header
  }
  | IDENTIFIER ':' IDENTIFIER {
      param_error = 1;
      yyerror("no type defined");
      $$ = create_node("ERROR", 0);
  }
;

ret_type
  : INT { strcpy(return_type_to_push, "INT"); $$ = create_node("INT", 0); }
  | REAL { strcpy(return_type_to_push, "REAL"); $$ = create_node("REAL", 0); }
  | CHAR { strcpy(return_type_to_push, "CHAR"); $$ = create_node("CHAR", 0); }
  | BOOL { strcpy(return_type_to_push, "BOOL"); $$ = create_node("BOOL", 0); }
  | INT_PTR { strcpy(return_type_to_push, "INT_PTR"); $$ = create_node("INT_PTR", 0); }
  | REAL_PTR { strcpy(return_type_to_push, "REAL_PTR"); $$ = create_node("REAL_PTR", 0); }
  | CHAR_PTR { strcpy(return_type_to_push, "CHAR_PTR"); $$ = create_node("CHAR_PTR", 0); }
  | STRING { 
      string_return_error = 1;
      yyerror("Error: Functions cannot return type string");
      strcpy(return_type_to_push, ""); // Set empty to avoid further errors
      $$ = create_node("ERROR", 0); 
    }
;

type
  : INT { $$ = create_node("INT", 0); }
  | REAL { $$ = create_node("REAL", 0); }
  | CHAR { $$ = create_node("CHAR", 0); }
  | BOOL { $$ = create_node("BOOL", 0); }
  | STRING { $$ = create_node("STRING", 0); }
  | INT_PTR { $$ = create_node("INT_PTR", 0); }
  | REAL_PTR { $$ = create_node("REAL_PTR", 0); }
  | CHAR_PTR { $$ = create_node("CHAR_PTR", 0); }
;

block
  : BEGIN_T inner_block END_T { $$ = $2; }
  | BEGIN_T END_T { $$ = create_node("BLOCK", 0); }
;


body 
  : VAR optional_var_list block { $$ = create_node("VAR_BLOCK", 2, $2, $3); }
  | block { $$ = $1; }
;

inner_block
  : nested_func inner_block {
      if ($2 == NULL)
          $$ = create_node("BLOCK", 1, $1);
      else {
          Node* merged = create_node("BLOCK", $2->child_count + 1);
          merged->children[0] = $1;
          for (int i = 0; i < $2->child_count; i++)
              merged->children[i+1] = $2->children[i];
          $$ = merged;
      }
  }
  | stmts inner_block {
      if ($2 == NULL)
          $$ = create_node("BLOCK", 1, $1);
      else {
          Node* merged = create_node("BLOCK", $2->child_count + 1);
          merged->children[0] = $1;
          for (int i = 0; i < $2->child_count; i++)
              merged->children[i+1] = $2->children[i];
          $$ = merged;
      }
  }
  | nested_func { $$ = create_node("BLOCK", 1, $1); }
  | stmts { $$ = create_node("BLOCK", 1, $1); }
  | /* empty */ { $$ = create_node("BLOCK", 0); }
;

stmts
  : stmt { $$ = $1; }
  | stmt stmts {
      if (strcmp($2->name, "BLOCK") == 0) {
          Node* merged = create_node("BLOCK", $2->child_count + 1);
          merged->children[0] = $1;
          for (int i = 0; i < $2->child_count; i++)
              merged->children[i+1] = $2->children[i];
          $$ = merged;
      } else {
          $$ = create_node("BLOCK", 2, $1, $2);
      }
  }
;

stmt
  : RETURN expr ';' {
      current_func_has_return = 1;
      if (!check_return_type($2)) {
          yyerror("Return type mismatch or returning from procedure");
      }
      $$ = create_node("RET", 1, $2);
  }
  | RETURN ';' {
      current_func_has_return = 1;
      if (strlen(current_return_type) > 0)
          yyerror("Function with return type must return a value");
      $$ = create_node("RET", 0);
  }
  | IDENTIFIER ASSIGN expr ';' {
    check_var_use($1, yylineno);
    $$ = create_node("=", 2, create_node($1, 0), $3);
  }
  | IDENTIFIER ASSIGN CALL IDENTIFIER '(' args ')' ';' {
    check_var_use($1, yylineno);
    
    int func_found = check_function_call($4, yylineno);
    
    // Count arguments
    int arg_count = 0;
    if (strcmp($6->name, "ARGS NONE") != 0) {
        arg_count = $6->child_count;
    }
    
    // Check parameter count even if function is not declared
    int required_count = get_function_param_count($4);
    if (required_count >= 0 && required_count != arg_count) {
        param_count_mismatch = 1;
        sprintf(yytext, "%s", $4);  // Store function name for error
        yylineno = @4.first_line;   // Use the line number of function name token
        yyerror("Parameter count mismatch");
    } 
    else if (!func_found) {
        // Only trigger undeclared function error if param count was correct
        undeclared_function_error = 1;
        sprintf(yytext, "%s", $4);
        yylineno = @4.first_line;
        yyerror("Function called before declaration");
    }
    else {
        // If function exists and param count is correct, check param types
        check_param_types($4, $6, yylineno);
    }
    
    Node* call_node = create_node("CALL", 2, create_node($4, 0), $6);
    $$ = create_node("=", 2, create_node($1, 0), call_node);
}
  | CALL IDENTIFIER '(' args ')' ';' {
    int func_found = check_function_call($2, yylineno);
    
    // Count arguments
    int arg_count = 0;
    if (strcmp($4->name, "ARGS NONE") != 0) {
        arg_count = $4->child_count;
    }
    
    // Check parameter count even if function is not declared
    int required_count = get_function_param_count($2);
    if (required_count >= 0 && required_count != arg_count) {
        param_count_mismatch = 1;
        sprintf(yytext, "%s", $2);  // Store function name for error
        yylineno = @2.first_line;  // Use the line number of function name token
        yyerror("Parameter count mismatch");
    } 
    else if (!func_found) {
        // Only trigger undeclared function error if param count was correct
        undeclared_function_error = 1;
        sprintf(yytext, "%s", $2);
        yylineno = @2.first_line;
        yyerror("Function called before declaration");
    }
    else {
        // If function exists and param count is correct, check param types
        check_param_types($2, $4, yylineno);
    }
    
    $$ = create_node("CALL", 2, create_node($2, 0), $4);
}
  | IDENTIFIER '[' expr ']' ASSIGN expr ';' {
    check_var_use($1, yylineno);
    if (is_string_expr($3)) {
        string_index_error = 1;
        yyerror("String expressions cannot be used as array indices");
    }
    else if (!is_int_expr($3)) {
        array_index_error = 1;
        yyerror("Array index must be of integer type");
    }
    
    // Get variable type and debug it
    char* var_type = get_var_type($1);
    printf("DEBUG: Assigning to array '%s' of type '%s'\n", 
           $1, var_type ? var_type : "unknown");
    
    // Debug the expression being assigned
    debug_node($6, "Expression being assigned");
    
    // Check if we're assigning a number or numeric array to a string array
    if (var_type && strcmp(var_type, "STRING") == 0) {
        // If it's an array element access
        if (strcmp($6->name, "ARRAY_ELEM") == 0) {
            char* src_array_name = $6->children[0]->name;
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
        else if ($6->child_count == 0 && isdigit($6->name[0])) {
            non_string_array_access_error = 1;
            yyerror("Cannot assign numeric value to string array element");
        }
    }
    
    Node* arr_elem = create_node("ARRAY_ELEM", 2, create_node($1, 0), $3);
    $$ = create_node("=", 2, arr_elem, $6);
  }
  | MUL IDENTIFIER ASSIGN expr ';' {
    check_var_use($2, yylineno);
    
    // Get variable type
    char* var_type = get_var_type($2);
    
    // Check if it's a pointer type
    if (var_type) {
        if (strcmp(var_type, "INT_PTR") != 0 && 
            strcmp(var_type, "REAL_PTR") != 0 && 
            strcmp(var_type, "CHAR_PTR") != 0) {
            // Error: Can't dereference non-pointer
            dereference_type_error = 1;
            sprintf(yytext, "%s", $2);
            yyerror("Cannot dereference non-pointer variable");
        }
    }
    
    $$ = create_node("= *", 2, create_node($2, 0), $4);
  }
  | IF expr ':' body ELSE ':' body {
      if (!is_bool_expr($2)) {
          if_expr_type_error = 1;
          yyerror("If condition must be a boolean expression");
      }
      $$ = create_node("IF-ELSE", 3, $2, $4, $7);
  }
  | IF expr ':' body ELIF expr ':' body ELSE ':' body {
      if (!is_bool_expr($2) || !is_bool_expr($6)) {
          if_expr_type_error = 1;
          yyerror("If/elif condition must be a boolean expression");
      }
      $$ = create_node("IF-ELIF-ELSE", 5, $2, $4, $6, $8, $11);
  }
  | IF expr ':' body ELIF expr ':' body {
      if (!is_bool_expr($2) || !is_bool_expr($6)) {
          if_expr_type_error = 1;
          yyerror("If/elif condition must be a boolean expression");
      }
      $$ = create_node("IF-ELIF", 4, $2, $4, $6, $8);
  }
  | IF expr ':' body {
      if (!is_bool_expr($2)) {
          if_expr_type_error = 1;
          yyerror("If condition must be a boolean expression");
      }
      $$ = create_node("IF", 2, $2, $4);
  }
  | WHILE expr ':' body {
      if (!is_bool_expr($2)) {
          if_expr_type_error = 1;
          yyerror("While condition must be a boolean expression");
      }
      $$ = create_node("WHILE", 2, $2, $4);
  }
  | DO ':' body WHILE expr ';' {
      if (!is_bool_expr($5)) {
          if_expr_type_error = 1;
          yyerror("Do-while condition must be a boolean expression");
      }
      $$ = create_node("DO-WHILE", 2, $3, $5);
  }
  | FOR '(' expr ';' expr ';' expr ')' ':' body {
      if (!is_bool_expr($5)) {
          if_expr_type_error = 1;
          yyerror("For condition must be a boolean expression");
      }
      Node* cond = create_node("FOR_COND", 3, $3, $5, $7);
      $$ = create_node("FOR", 2, cond, $10);
  }
  | VAR optional_var_list block {
      $$ = create_node("BODY", 2, $2, $3);
  }
  | block {
      $$ = $1;
  }
  | VAR optional_var_list stmt {
      $$ = create_node("VAR_BLOCK", 2, $2, $3);
  }
  | '(' IDENTIFIER ADD expr ')' '[' expr ']' ASSIGN expr ';' {
      check_var_use($2, yylineno);
      array_pointer_arithmetic_error = 1;
      yyerror("Cannot add anything to array elements - they are not pointers");
      $$ = create_node("ERROR", 0);
  }
  | '(' IDENTIFIER SUB expr ')' '[' expr ']' ASSIGN expr ';' {
      check_var_use($2, yylineno);
      array_pointer_arithmetic_error = 1;
      yyerror("Cannot add anything to array elements - they are not pointers");
      $$ = create_node("ERROR", 0);
  }
;
/* הצהרות משתנים (var_decls וכו') */

var_decls
  : VAR optional_var_list { $$ = $2; }
  | /* empty */ { $$ = create_node("BLOCK", 0); }
;

optional_var_list
  : var_decl_list { $$ = $1; }
  | /* empty */ { $$ = create_node("BLOCK", 0); }
;

var_decl_list
  : var_decl_list var_single_decl {
      Node* merged = create_node("BLOCK", $1->child_count + 1);
      for (int i = 0; i < $1->child_count; i++)
          merged->children[i] = $1->children[i];
      merged->children[$1->child_count] = $2;
      $$ = merged;
  }
  | var_single_decl { $$ = create_node("BLOCK", 1, $1); }
;

var_single_decl
  : TYPE BOOL ':' IDENTIFIER ',' IDENTIFIER ';' {
      // Handle this specific case: type bool:c,d;
      add_var($4, "BOOL");  // Add first variable with type
      add_var($6, "BOOL");  // Add second variable with type
      
      // Create nodes for both variables
      char temp1[100], temp2[100];
      sprintf(temp1, "BOOL %s", $4);
      sprintf(temp2, "BOOL %s", $6);
      
      // Create a list of bool declarations
      Node* var_list = create_node("VAR_LIST", 2);
      var_list->children[0] = create_node(temp1, 0);
      var_list->children[1] = create_node(temp2, 0);
      
      $$ = var_list;
  }
  | TYPE type ':' id_list ';' {
      // Regular type declarations without initialization
      char* type_name = $2->name;
      
      if ($4->child_count == 0) {
          // Single variable declaration
          add_var($4->name, type_name);  // Add with type
          
          char temp[100];
          sprintf(temp, "%s %s", type_name, $4->name);
          $$ = create_node(temp, 0);
      } else {
          // Multiple variable declarations
          Node* var_list = create_node("VAR_LIST", $4->child_count);
          for (int i = 0; i < $4->child_count; i++) {
              add_var($4->children[i]->name, type_name);  // Add each with type
              
              char temp[100];
              sprintf(temp, "%s %s", type_name, $4->children[i]->name);
              var_list->children[i] = create_node(temp, 0);
          }
          $$ = var_list;
      }
  }
  | TYPE type ':' IDENTIFIER ':' expr ';' {
      // Variable with initialization
      add_var($4, $2->name);  // Add variable with type
      
      char temp[100];
      sprintf(temp, "%s %s", $2->name, $4);
      Node* var_node = create_node(temp, 0);
      $$ = create_node("=", 2, var_node, $6);
  }
  | TYPE BOOL ':' var_init_list ';' {
      // This is handled in var_init_list
      $$ = $4;
  }
  | TYPE BOOL ':' IDENTIFIER ';' {  // Single boolean without initialization
      add_var($4, "BOOL");  // Add with type
      
      char temp[100];
      sprintf(temp, "BOOL %s", $4);
      $$ = create_node(temp, 0);
  }
  | TYPE type ':' string_decl_list ';' {
    // Process each array declaration with the correct type
    for (int i = 0; i < $4->child_count; i++) {
        Node* array_node = $4->children[i];
        
        // Extract array name from the node name 
        if (strncmp(array_node->name, "STR ", 4) == 0) {
            char* name_start = array_node->name + 4;
            char* bracket_pos = strchr(name_start, '[');
            if (bracket_pos) {
                size_t name_len = bracket_pos - name_start;
                char* array_name = (char*)malloc(name_len + 1);
                strncpy(array_name, name_start, name_len);
                array_name[name_len] = '\0';
                
                printf("Adding array '%s' with type '%s'\n", array_name, $2->name);
                
                // Add the array with its correct type
                add_var(array_name, $2->name);
                free(array_name);
            }
        }
    }
    $$ = $4;
}
;

id_list
  : IDENTIFIER {
      $$ = create_node($1, 0);  
  }
  | IDENTIFIER ',' id_list {
      // Create a node for this identifier
      Node* id_node = create_node($1, 0);
      
      // Create a list
      Node* merged;
      if ($3->child_count == 0) {
          merged = create_node("ID_LIST", 2, id_node, $3);
      } else {
          merged = create_node("ID_LIST", $3->child_count + 1);
          merged->children[0] = id_node;
          for (int i = 0; i < $3->child_count; i++) {
              merged->children[i+1] = $3->children[i];
          }
      }
      $$ = merged;
  }
;


var_init_list
  : IDENTIFIER ':' expr {
      add_var($1, "BOOL");  // Add boolean variable
      
      char temp[100];
      sprintf(temp, "BOOL %s", $1);
      Node* var_node = create_node(temp, 0);
      $$ = create_node("=", 2, var_node, $3);
  }
  | IDENTIFIER ':' expr ',' var_init_list {
      add_var($1, "BOOL");  // Add boolean variable
      
      char temp[100];
      sprintf(temp, "BOOL %s", $1);
      Node* var_node = create_node(temp, 0);
      Node* assign_node = create_node("=", 2, var_node, $3);
      Node* merged = create_node("BLOCK", $5->child_count + 1);
      merged->children[0] = assign_node;
      for (int i = 0; i < $5->child_count; i++)
          merged->children[i+1] = $5->children[i];
      $$ = merged;
  }
;

/* אתחול משתנים מסוג מחרוזת */
string_decl_list
  : string_decl_list ',' string_decl {
      Node* merged = create_node("BLOCK", $1->child_count + 1);
      for (int i = 0; i < $1->child_count; i++)
          merged->children[i] = $1->children[i];
      merged->children[$1->child_count] = $3;
      $$ = merged;
  }
  | string_decl {
      $$ = create_node("BLOCK", 1, $1);
  }
;

string_decl
  : IDENTIFIER '[' INT_LITERAL ']' {
      char temp[100];
      sprintf(temp, "STR %s[%s]", $1, $3);
      $$ = create_node(temp, 0);
      // Do NOT add the variable here - it will be added in the parent rule
  }
  | IDENTIFIER '[' INT_LITERAL ']' ':' STRING_LITERAL {
      char temp[100];
      sprintf(temp, "STR %s[%s]:%s", $1, $3, $6);
      $$ = create_node(temp, 0);
      // Do NOT add the variable here - it will be added in the parent rule
  }
;

/* ביטויים מתמטיים ולוגיים */
expr 
  : expr ADD expr { 
    Node* node = create_node("+", 2, $1, $3);
    
    // Type checking
    char* left_type = get_expr_type($1);
    char* right_type = get_expr_type($3);
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            arith_type_error = 1;
            yyerror("Arithmetic operators require numeric operands");
        }
    }
    
    $$ = node;
  }
  | expr SUB expr { 
    Node* node = create_node("-", 2, $1, $3);
    
    // Type checking
    char* left_type = get_expr_type($1);
    char* right_type = get_expr_type($3);
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            arith_type_error = 1;
            yyerror("Arithmetic operators require numeric operands");
        }
    }
    
    $$ = node;
  }
  | expr MUL expr { 
    Node* node = create_node("*", 2, $1, $3);
    
    // Type checking
    char* left_type = get_expr_type($1);
    char* right_type = get_expr_type($3);
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            arith_type_error = 1;
            yyerror("Arithmetic operators require numeric operands");
        }
    }
    
    $$ = node;
  }
  | expr DIV expr { 
    Node* node = create_node("/", 2, $1, $3);
    
    // Type checking
    char* left_type = get_expr_type($1);
    char* right_type = get_expr_type($3);
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            arith_type_error = 1;
            yyerror("Arithmetic operators require numeric operands");
        }
    }
    
    $$ = node;
  }
  | expr EQ expr { 
    Node* node = create_node("==", 2, $1, $3);
    
    // Type checking
    char* left_type = get_expr_type($1);
    char* right_type = get_expr_type($3);
    
    if (left_type && right_type) {
        // Types must be compatible
        if (strcmp(left_type, right_type) != 0 &&
            !((strcmp(left_type, "INT") == 0 && strcmp(right_type, "REAL") == 0) ||
              (strcmp(left_type, "REAL") == 0 && strcmp(right_type, "INT") == 0))) {
            equality_type_error = 1;
            yyerror("Equality operators require compatible types");
        }
    }
    
    $$ = node;
  }
  | expr NE expr { 
    Node* node = create_node("!=", 2, $1, $3);
    
    // Type checking
    char* left_type = get_expr_type($1);
    char* right_type = get_expr_type($3);
    
    if (left_type && right_type) {
        // Types must be compatible
        if (strcmp(left_type, right_type) != 0 &&
            !((strcmp(left_type, "INT") == 0 && strcmp(right_type, "REAL") == 0) ||
              (strcmp(left_type, "REAL") == 0 && strcmp(right_type, "INT") == 0))) {
            equality_type_error = 1;
            yyerror("Equality operators require compatible types");
        }
    }
    
    $$ = node;
  }
  | expr GT expr { 
    Node* node = create_node(">", 2, $1, $3);
    
    // Type checking
    char* left_type = get_expr_type($1);
    char* right_type = get_expr_type($3);
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            comparison_type_error = 1;
            yyerror("Comparison operators require numeric operands");
        }
    }
    
    $$ = node;
  }
  | expr GE expr { 
    Node* node = create_node(">=", 2, $1, $3);
    
    // Type checking
    char* left_type = get_expr_type($1);
    char* right_type = get_expr_type($3);
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            comparison_type_error = 1;
            yyerror("Comparison operators require numeric operands");
        }
    }
    
    $$ = node;
  }
  | expr LT expr { 
    Node* node = create_node("<", 2, $1, $3);
    
    // Type checking
    char* left_type = get_expr_type($1);
    char* right_type = get_expr_type($3);
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            comparison_type_error = 1;
            yyerror("Comparison operators require numeric operands");
        }
    }
    
    $$ = node;
  }
  | expr LE expr { 
    Node* node = create_node("<=", 2, $1, $3);
    
    // Type checking
    char* left_type = get_expr_type($1);
    char* right_type = get_expr_type($3);
    
    if (left_type && right_type) {
        // Both operands must be numeric (INT or REAL)
        if ((strcmp(left_type, "INT") != 0 && strcmp(left_type, "REAL") != 0) ||
            (strcmp(right_type, "INT") != 0 && strcmp(right_type, "REAL") != 0)) {
            comparison_type_error = 1;
            yyerror("Comparison operators require numeric operands");
        }
    }
    
    $$ = node;
  }
  
  | expr AND expr { 
      Node* node = create_node("AND", 2, $1, $3);
      // Type checking...
      char* left_type = get_expr_type($1);
      char* right_type = get_expr_type($3);
      
      if (left_type && right_type) {
          if (strcmp(left_type, "BOOL") != 0 || strcmp(right_type, "BOOL") != 0) {
              logical_op_type_error = 1;
              yyerror("Logical operators require boolean operands");
          }
      }
      $$ = node;
  }
  | expr BITWISE_AND expr { 
      Node* node = create_node("BITWISE_AND", 2, $1, $3);
      // Type checking...
      char* left_type = get_expr_type($1);
      char* right_type = get_expr_type($3);
      
      if (left_type && right_type) {
          if (strcmp(left_type, "BOOL") != 0 || strcmp(right_type, "BOOL") != 0) {
              logical_op_type_error = 1;
              yyerror("Logical && operator requires boolean operands");
          }
      }
      $$ = node;
  }
  | expr OR expr { 
      Node* node = create_node("OR", 2, $1, $3);
      // Type checking...
      char* left_type = get_expr_type($1);
      char* right_type = get_expr_type($3);
      
      if (left_type && right_type) {
          if (strcmp(left_type, "BOOL") != 0 || strcmp(right_type, "BOOL") != 0) {
              logical_op_type_error = 1;
              yyerror("Logical operators require boolean operands");
          }
      }
      $$ = node;
  }
  | expr BITWISE_OR expr {
      Node* node = create_node("BITWISE_OR", 2, $1, $3);
      // Type checking...
      char* left_type = get_expr_type($1);
      char* right_type = get_expr_type($3);
      
      if (left_type && right_type) {
          if (strcmp(left_type, "BOOL") != 0 || strcmp(right_type, "BOOL") != 0) {
              logical_op_type_error = 1;
              yyerror("Logical || operator requires boolean operands");
          }
      }
      $$ = node;
  }
  | NOT expr { 
      Node* node = create_node("NOT", 1, $2);
      // Type checking...
      char* operand_type = get_expr_type($2);
      
      if (operand_type) {
          if (strcmp(operand_type, "BOOL") != 0) {
              logical_op_type_error = 1;
              yyerror("NOT operator requires a boolean operand");
          }
      }
      $$ = node;
  }
  | BITWISE_NOT expr { 
      Node* node = create_node("BITWISE_NOT", 1, $2);
      
      // Type checking - operand must be boolean
      char* operand_type = get_expr_type($2);
      
      if (operand_type) {
          if (strcmp(operand_type, "BOOL") != 0) {
              logical_op_type_error = 1;
              yyerror("Bitwise NOT operator requires a boolean operand");
          }
      }
      
      $$ = node;
  }
  
  | '(' expr ')' { $$ = $2; }
  | SUB expr %prec UMINUS { $$ = create_node("-", 2, create_node("0", 0), $2); }
  | MUL IDENTIFIER { 
    check_var_use($2, yylineno);
    
    // Get variable type
    char* var_type = get_var_type($2);
    
    // Check if it's a pointer type
    if (var_type) {
        if (strcmp(var_type, "INT_PTR") != 0 && 
            strcmp(var_type, "REAL_PTR") != 0 && 
            strcmp(var_type, "CHAR_PTR") != 0) {
            // Error: Can't dereference non-pointer
            dereference_type_error = 1;
            sprintf(yytext, "%s", $2);
            yyerror("Cannot dereference non-pointer variable");
        }
    }
    
    $$ = create_node("*", 1, create_node($2, 0)); 
  }
 
  | '(' IDENTIFIER ADD expr ')' '[' expr ']' { 
      check_var_use($2, yylineno);
      array_pointer_arithmetic_error = 1;
      yylineno = @6.first_line;  // Line of the '[' token
      yyerror("Cannot add anything to array elements - they are not pointers");
      $$ = create_node("ERROR", 0);
  }
  | '(' IDENTIFIER SUB expr ')' '[' expr ']' { 
      check_var_use($2, yylineno);
      array_pointer_arithmetic_error = 1;
      yyerror("Cannot add anything to array elements - they are not pointers");
      $$ = create_node("ERROR", 0);
  }
  | ADDR IDENTIFIER { 
    check_var_use($2, yylineno);
    
    // Get the variable type
    char* var_type = get_var_type($2);
    
    // Check if it's a valid type for address-of
    if (var_type) {
        if (strcmp(var_type, "INT") != 0 && 
            strcmp(var_type, "REAL") != 0 && 
            strcmp(var_type, "CHAR") != 0 && 
            strcmp(var_type, "STRING") != 0) {
            // Error: Can't take address of this type
            printf("Semantic error at line %d: Cannot take address of variable of type '%s'\n", 
                   yylineno, var_type);
            yytext = $2; // Store the variable name for error message
            yyerror("Cannot take address of this type");
        }
    }
    
    $$ = create_node("&", 1, create_node($2, 0)); 
  }
  | ADDR IDENTIFIER '[' expr ']' { 
    check_var_use($2, yylineno);
    
    // Check if index is valid (integer)
    if (!is_int_expr($4)) {
        array_index_error = 1;
        yyerror("Array index must be of integer type");
    }
    
    // Create array element node
    Node* arr_elem = create_node("ARRAY_ELEM", 2, create_node($2, 0), $4);
    
    // Create address-of node
    $$ = create_node("&", 1, arr_elem); 
  }
  | ADDR '(' expr ')' { 
    // This should always produce an error
    yyerror("Cannot take address of an expression");
    $$ = create_node("ERROR", 0);
  }
  | MUL '(' expr ')' { 
    // Type checking - operand must be a pointer
    char* expr_type = get_expr_type($3);
    
    if (expr_type) {
        if (strcmp(expr_type, "INT_PTR") != 0 && 
            strcmp(expr_type, "REAL_PTR") != 0 && 
            strcmp(expr_type, "CHAR_PTR") != 0) {
            // Error: Can't dereference non-pointer
            dereference_type_error = 1;
            yyerror("Cannot dereference non-pointer expression");
        }
    }
    
    $$ = create_node("*", 1, $3); 
  }
  | IDENTIFIER { 
    check_var_use($1, yylineno);
    $$ = create_node($1, 0); 
  }
  | IDENTIFIER '(' args ')' { 
    int func_found = check_function_call($1, yylineno);
    
    // Count arguments
    int arg_count = 0;
    if (strcmp($3->name, "ARGS NONE") != 0) {
        arg_count = $3->child_count;
    }
    
    // Check parameter count even if function is not declared
    int required_count = get_function_param_count($1);
    if (required_count >= 0 && required_count != arg_count) {
        param_count_mismatch = 1;
        sprintf(yytext, "%s", $1);  // Store function name for error
        yylineno = @1.first_line;   // Use the line number of function name token
        yyerror("Parameter count mismatch");
    } 
    else if (!func_found) {
        // Only trigger undeclared function error if param count was correct
        undeclared_function_error = 1;
        sprintf(yytext, "%s", $1);
        yylineno = @1.first_line;
        yyerror("Function called before declaration");
    }
    else {
        // If function exists and param count is correct, check param types
        check_param_types($1, $3, yylineno);
    }
    
    $$ = create_node("CALL", 2, create_node($1, 0), $3); 
  }
  | IDENTIFIER '[' expr ']' { 
    check_var_use($1, yylineno);
    
    // Check if the index expression is a REAL literal or identifier
    if ($3->child_count == 0) {
        // Check if it's a REAL literal
        if (strchr($3->name, '.')) {
            array_index_error = 1;
            yyerror("Array index must be of integer type");
        }
        
        // Check if it's a variable of type REAL
        char* var_type = get_var_type($3->name);
        if (var_type && strcmp(var_type, "REAL") == 0) {
            array_index_error = 1;
            yyerror("Array index must be of integer type");
        }
    }
    
    // String expressions as array indices
    if (is_string_expr($3)) {
        string_index_error = 1;
        yyerror("String expressions cannot be used as array indices");
    }
    
    $$ = create_node("ARRAY_ELEM", 2, create_node($1, 0), $3); 
  }
  | IDENTIFIER { 
    check_var_use($1, yylineno);
    $$ = create_node($1, 0); 
  }
  | INT_LITERAL { $$ = create_node($1, 0); }
  | REAL_LITERAL { $$ = create_node($1, 0); }
  | CHAR_LITERAL { $$ = create_node($1, 0); }
  | STRING_LITERAL { $$ = create_node($1, 0); }
  | TRUE { $$ = create_node("TRUE", 0); }
  | FALSE { $$ = create_node("FALSE", 0); }
  | NULL_T { $$ = create_node("NULL", 0); }
  | PIPE_SYMBOL IDENTIFIER PIPE_SYMBOL { 
	    check_var_use($2, yylineno);
	    
	    // Type checking - the operand must be a string
	    char* var_type = get_var_type($2);
	    if (var_type && strcmp(var_type, "STRING") != 0) {
		length_type_error = 1;
		yyerror("Length operator requires a string operand");
	    }
	    
	    $$ = create_node("LEN", 1, create_node($2, 0)); 
  }
  | '(' expr ADD expr ')' '[' expr ']' {
    array_pointer_arithmetic_error = 1;
    yyerror("Cannot add anything to array elements - they are not pointers");
    $$ = create_node("ARRAY_ELEM", 3, $2, $4, $7);
  }
;

/* קריאות לפונקציה: ארגומנטים */
args 
  : expr { $$ = create_node("ARGS", 1, $1); }
  | expr ',' args {
      if (strcmp($3->name, "ARGS") == 0) {
          Node* merged = create_node("ARGS", $3->child_count + 1);
          merged->children[0] = $1;
          for (int i = 0; i < $3->child_count; i++)
              merged->children[i+1] = $3->children[i];
          $$ = merged;
      } else {
          $$ = create_node("ARGS", 2, $1, $3);
      }
  }
  | /* empty */ { $$ = create_node("ARGS NONE", 0); }
;

%%

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

/* MAIN שמריץ את ה-parser */
int main() {
    printf("Parsing completed successfully.\n");
    
    if (yyparse() == 0) {
        if (!has_main) {
            fprintf(stderr, "Error: Program must have exactly one _main_() procedure\n");
            return 1;
        }

        // Debug all variables
        debug_print_all_vars();
        printf("Starting AST generation...\n");

        print_ast(root, 0); // הדפסת AST

        // הוספה כאן — יצירת קוד ביניים
        printf("Generating 3AC...\n");
	gen_code(root);
    }
    return 0;
}
