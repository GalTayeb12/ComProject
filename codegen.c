#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

int temp_id = 0;
int label_id = 1;
int function_start_temp_id = 0; // Track temp usage per function

// Forward declarations
char* gen_expr(Node* node);

char* new_temp() {
    char* temp = (char*)malloc(10);
    sprintf(temp, "t%d", temp_id++);
    return temp;
}

char* new_label() {
    char* label = (char*)malloc(10);
    sprintf(label, "L%d", label_id++);
    return label;
}

int size_of_type(const char* type) {
    if (strcmp(type, "INT") == 0) return 4;
    if (strcmp(type, "REAL") == 0) return 8;
    if (strcmp(type, "CHAR") == 0) return 1;
    if (strcmp(type, "BOOL") == 0) return 1;
    if (strcmp(type, "STRING") == 0) return 8; // pointer
    if (strstr(type, "_PTR") != NULL) return 8;
    return 4; // default
}

int sum_local_var_size(Node* decls) {
    if (!decls) return 0;
    int total = 0;

    if (strcmp(decls->name, "VAR_LIST") == 0 || strcmp(decls->name, "BLOCK") == 0) {
        for (int i = 0; i < decls->child_count; i++) {
            Node* child = decls->children[i];
            if (child->child_count == 0) {
                // Parse "TYPE varname" format
                char* copy = strdup(child->name);
                char* type = strtok(copy, " ");
                total += size_of_type(type);
                free(copy);
            } else {
                // Handle nested structures
                total += sum_local_var_size(child);
            }
        }
    } else if (decls->child_count == 0) {
        // Single variable declaration
        char* copy = strdup(decls->name);
        char* type = strtok(copy, " ");
        total += size_of_type(type);
        free(copy);
    } else {
        // Recursively handle child nodes
        for (int i = 0; i < decls->child_count; i++) {
            total += sum_local_var_size(decls->children[i]);
        }
    }

    return total;
}

// Helper function to get the size of a parameter based on its type
int get_param_size(Node* arg) {
    if (!arg) return 4; // default
    
    if (arg->child_count == 0) {
        // Check if it's a literal
        if (isdigit(arg->name[0]) && !strchr(arg->name, '.')) {
            return 4; // INT literal
        } else if (strchr(arg->name, '.')) {
            return 8; // REAL literal
        } else if (arg->name[0] == '\'') {
            return 1; // CHAR literal
        } else if (strcmp(arg->name, "TRUE") == 0 || strcmp(arg->name, "FALSE") == 0) {
            return 1; // BOOL literal
        } else if (arg->name[0] == '"') {
            return 8; // STRING literal (pointer)
        } else {
            // Variable - we'd need to look up its type in the symbol table
            // For now, assume INT (4 bytes) as default
            return 4;
        }
    }
    
    return 4; // default for expressions
}

// Helper function to calculate total parameter size in bytes
int calculate_param_bytes(Node* args) {
    if (!args || strcmp(args->name, "ARGS NONE") == 0) {
        return 0;
    }
    
    int total_bytes = 0;
    for (int i = 0; i < args->child_count; i++) {
        total_bytes += get_param_size(args->children[i]);
    }
    
    return total_bytes;
}
int is_literal(Node* node) {
    if (!node || node->child_count > 0) return 0;
    
    return (isdigit(node->name[0]) || 
            (node->name[0] == '-' && isdigit(node->name[1])) ||
            strchr(node->name, '.') != NULL ||
            strcmp(node->name, "TRUE") == 0 || 
            strcmp(node->name, "FALSE") == 0 || 
            node->name[0] == '\'' || 
            node->name[0] == '"');
}

void gen_stmt(Node* node) {
    if (!node) return;

    if (strcmp(node->name, "=") == 0 && node->child_count == 2) {
        char* rhs = gen_expr(node->children[1]);
        printf("    %s = %s\n", node->children[0]->name, rhs);
        free(rhs);
    }
    else if (strcmp(node->name, "RET") == 0 && node->child_count == 1) {
        char* val = gen_expr(node->children[0]);
        printf("    Return %s\n", val);
        free(val);
    }
    else if (strcmp(node->name, "IF-ELSE") == 0 && node->child_count == 3) {
        // Special handling for conditions - don't create temps for simple comparisons
        Node* condition = node->children[0];
        char* label_true = new_label();
        char* label_false = new_label();
        char* label_end = new_label();

        // Handle simple comparisons directly
        if (condition->child_count == 2 && 
            (strcmp(condition->name, "==") == 0 || strcmp(condition->name, "!=") == 0 ||
             strcmp(condition->name, "<") == 0 || strcmp(condition->name, ">") == 0 ||
             strcmp(condition->name, "<=") == 0 || strcmp(condition->name, ">=") == 0)) {
            
            // For simple comparisons, get the operands without creating temps
            char* left_name = condition->children[0]->name;
            char* right_name = condition->children[1]->name;
            printf("    if %s %s %s Goto %s\n", left_name, condition->name, right_name, label_true);
        } else {
            char* cond = gen_expr(condition);
            printf("    if %s Goto %s\n", cond, label_true);
            free(cond);
        }
        
        printf("    Goto %s\n", label_false);
        printf("%s:\n", label_true);
        gen_stmt(node->children[1]);
        printf("    Goto %s\n", label_end);
        printf("%s:\n", label_false);
        gen_stmt(node->children[2]);
        printf("%s:\n", label_end);

        free(label_true); free(label_false); free(label_end);
    }
    else if (strcmp(node->name, "WHILE") == 0 && node->child_count == 2) {
        char* label_start = new_label();
        char* label_body = new_label();
        char* label_end = new_label();

        printf("%s:\n", label_start);
        
        // Handle simple comparisons directly for while conditions too
        Node* condition = node->children[0];
        if (condition->child_count == 2 && 
            (strcmp(condition->name, "==") == 0 || strcmp(condition->name, "!=") == 0 ||
             strcmp(condition->name, "<") == 0 || strcmp(condition->name, ">") == 0 ||
             strcmp(condition->name, "<=") == 0 || strcmp(condition->name, ">=") == 0)) {
            
            // For simple comparisons, get the operands without creating temps
            char* left_name = condition->children[0]->name;
            char* right_name = condition->children[1]->name;
            printf("    if %s %s %s Goto %s\n", left_name, condition->name, right_name, label_body);
        } else {
            char* cond = gen_expr(condition);
            printf("    if %s Goto %s\n", cond, label_body);
            free(cond);
        }
        
        printf("    Goto %s\n", label_end);
        printf("%s:\n", label_body);
        gen_stmt(node->children[1]);
        printf("    Goto %s\n", label_start);
        printf("%s:\n", label_end);

        free(label_start); free(label_body); free(label_end);
    }
    else if (strcmp(node->name, "CALL") == 0 && node->child_count == 2) {
        // Handle standalone function calls (not assigned to a variable)
        Node* func_name = node->children[0];
        Node* args = node->children[1];
        int arg_count = 0;
        
        // Handle arguments (push from right to left)
        if (strcmp(args->name, "ARGS NONE") != 0) {
            arg_count = args->child_count;
            // Push parameters from right to left (last parameter first)
            for (int i = arg_count - 1; i >= 0; i--) {
                char* arg = gen_expr(args->children[i]);
                printf("    PushParam %s\n", arg);
                free(arg);
            }
        }

        // Call the function (result not stored since it's a procedure call)
        char* temp = new_temp();
        printf("    %s = LCall %s\n", temp, func_name->name);
        
        // Calculate total bytes for PopParams
        int param_bytes = calculate_param_bytes(args);
        printf("    PopParams %d\n", param_bytes);
        free(temp);
    }
    else {
        for (int i = 0; i < node->child_count; i++) {
            gen_stmt(node->children[i]);
        }
    }
}

char* gen_expr(Node* node) {
    if (!node) return strdup("NULL");

    if (node->child_count == 0) {
        // For literals in assignments, create a temporary variable
        if (is_literal(node)) {
            char* temp = new_temp();
            printf("    %s = %s\n", temp, node->name);
            return temp;
        } else {
            // For identifiers, return as is
            return strdup(node->name);
        }
    }

    if (strcmp(node->name, "CALL") == 0 && node->child_count == 2) {
        Node* func_name = node->children[0];
        Node* args = node->children[1];
        int arg_count = args->child_count;

        // Push parameters from right to left (last parameter first)
        for (int i = arg_count - 1; i >= 0; i--) {
            char* arg = gen_expr(args->children[i]);
            printf("    PushParam %s\n", arg);
            free(arg);
        }

        char* temp = new_temp();
        printf("    %s = LCall %s\n", temp, func_name->name);
        
        // Calculate total bytes for PopParams
        int param_bytes = calculate_param_bytes(args);
        printf("    PopParams %d\n", param_bytes);
        return temp;
    }

    if (node->child_count == 2) {
        char* left = gen_expr(node->children[0]);
        char* right = gen_expr(node->children[1]);
        char* temp = new_temp();
        printf("    %s = %s %s %s\n", temp, left, node->name, right);
        free(left); free(right);
        return temp;
    }

    return strdup("UNKNOWN_EXPR");
}


void gen_func(Node* node) {
    if (!node || node->child_count < 3) return;

    const char* func_name = node->name;
    Node* params = node->children[0];
    Node* ret_type = node->children[1];
    Node* body = node->children[2];

    // Remember starting temp_id for this function
    function_start_temp_id = temp_id;
    
    // Calculate local variables size (only declared variables)
    int local_var_size = 0;
    if (body && strcmp(body->name, "BODY") == 0 && body->child_count >= 1) {
        Node* decls = body->children[0];
        local_var_size = sum_local_var_size(decls);
    }

    // Print function label
    printf("%s:\n", func_name);
    
    // Generate code for function body 
    Node* stmts = body->child_count == 2 ? body->children[1] : body;
    
    // Temporarily capture output to calculate temps, then print BeginFunc
    FILE* original_stdout = stdout;
    char temp_output[10000] = {0}; // Buffer to store generated code
    FILE* mem_file = fmemopen(temp_output, sizeof(temp_output), "w");
    
    if (mem_file) {
        stdout = mem_file;
        gen_stmt(stmts);
        fclose(mem_file);
        stdout = original_stdout;
        
        // Calculate how many temps were used
        int temps_used = temp_id - function_start_temp_id;
        int temp_var_size = temps_used * 4; // Each temp is 4 bytes (INT)
        int total_size = local_var_size + temp_var_size;
        
        // Print BeginFunc with correct size
        printf("    BeginFunc %d\n", total_size);
        
        // Print the generated code
        printf("%s", temp_output);
    } else {
        // Fallback if fmemopen fails
        printf("    BeginFunc %d\n", local_var_size + 20); // rough estimate
        gen_stmt(stmts);
    }

    printf("    EndFunc\n");
}

void gen_code(Node* root) {
    if (!root) {
        printf("DEBUG: gen_code got NULL root\n");
        return;
    }

    if (strcmp(root->name, "CODE") == 0 || strcmp(root->name, "FUNC") == 0) {
        for (int i = 0; i < root->child_count; i++) {
            gen_code(root->children[i]);
        }
    }
    else {
        gen_func(root);
    }
}
