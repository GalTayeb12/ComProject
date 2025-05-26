#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int temp_id = 0;
int label_id = 1;

char* new_temp();
char* new_label();
char* gen_expr(Node* node);

// Add these function prototypes:
int calculate_param_size(Node* args);
int calculate_local_var_size(Node* decls);
int count_local_vars(Node* decls);
int calculate_param_var_size(Node* params);

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

// Also improve size_of_type function
int size_of_type(const char* type) {
    if (strcmp(type, "INT") == 0) return 4;
    if (strcmp(type, "REAL") == 0) return 8;
    if (strcmp(type, "CHAR") == 0) return 1;
    if (strcmp(type, "BOOL") == 0) return 1;
    if (strcmp(type, "STRING") == 0) return 8; // pointer size
    if (strstr(type, "_PTR") != NULL) return 8; // all pointers are 8 bytes
    
    // Handle array declarations (for strings like "STRING varname[size]")
    if (strstr(type, "[") != NULL) {
        // Extract array size and calculate total
        char* bracket = strchr(type, '[');
        if (bracket) {
            int size = atoi(bracket + 1);
            return size; // for character arrays
        }
    }
    
    return 4; // default
}

int sum_local_var_size(Node* decls) {
    if (!decls) return 0;
    int total = 0;

    if (strcmp(decls->name, "VAR_LIST") == 0 || strcmp(decls->name, "BLOCK") == 0) {
        for (int i = 0; i < decls->child_count; i++) {
            Node* child = decls->children[i];
            if (child->child_count == 0) {
                // Parse variable declaration like "INT varname"
                char* copy = strdup(child->name);
                char* type = strtok(copy, " ");
                if (type) {
                    total += size_of_type(type);
                }
                free(copy);
            } else {
                // Handle nested declarations
                total += sum_local_var_size(child);
            }
        }
    } else if (decls->child_count == 0) {
        // Single variable declaration
        char* copy = strdup(decls->name);
        char* type = strtok(copy, " ");
        if (type) {
            total += size_of_type(type);
        }
        free(copy);
    } else {
        // Recursively handle complex structures
        for (int i = 0; i < decls->child_count; i++) {
            total += sum_local_var_size(decls->children[i]);
        }
    }

    return total;
}
int calculate_param_var_size(Node* params) {
    if (!params || strcmp(params->name, "PARS NONE") == 0) return 0;
    
    int total_size = 0;
    for (int i = 0; i < params->child_count; i++) {
        // Parse parameter string like "par1 REAL x"
        char* param_str = params->children[i]->name;
        char* copy = strdup(param_str);
        
        // Skip "par1" and get the type
        char* token = strtok(copy, " "); // "par1"
        token = strtok(NULL, " ");       // "REAL" or "INT" etc.
        
        if (token) {
            total_size += size_of_type(token);
        }
        free(copy);
    }
    return total_size;
}


char* gen_expr(Node* node);

void gen_stmt(Node* node) {
    if (!node) return;

    if (strcmp(node->name, "=") == 0 && node->child_count == 2) {
        Node* rhs_node = node->children[1];
        
        if (rhs_node->child_count == 0) {
            // Direct assignment for simple values
            printf("    %s = %s\n", node->children[0]->name, rhs_node->name);
        } else {
            // Complex expression
            char* rhs = gen_expr(rhs_node);
            printf("    %s = %s\n", node->children[0]->name, rhs);
            free(rhs);
        }
    }
    else if (strcmp(node->name, "RET") == 0 && node->child_count == 1) {
        char* val = gen_expr(node->children[0]);
        printf("    Return %s\n", val);
        free(val);
    }
    else if (strcmp(node->name, "RET") == 0 && node->child_count == 0) {
        printf("    Return\n");
    }
    else if (strcmp(node->name, "IF-ELSE") == 0 && node->child_count == 3) {
        char* cond = gen_expr(node->children[0]);
        char* label_true = new_label();
        char* label_false = new_label();
        char* label_end = new_label();

        printf("    if %s Goto %s\n", cond, label_true);
        printf("    Goto %s\n", label_false);
        printf(" %s:\n", label_true);
        gen_stmt(node->children[1]);
        printf("    Goto %s\n", label_end);
        printf(" %s:\n", label_false);
        gen_stmt(node->children[2]);
        printf(" %s:\n", label_end);

        free(cond); free(label_true); free(label_false); free(label_end);
    }
    else if (strcmp(node->name, "WHILE") == 0 && node->child_count == 2) {
        char* label_start = new_label();
        char* label_body = new_label();
        char* label_end = new_label();

        printf("%s:\n", label_start);
        char* cond = gen_expr(node->children[0]);
        printf("    if %s Goto %s\n", cond, label_body);
        printf("    Goto %s\n", label_end);
        printf(" %s:\n", label_body);
        gen_stmt(node->children[1]);
        printf("    Goto %s\n", label_start);
        printf(" %s:\n", label_end);

        free(cond); free(label_start); free(label_body); free(label_end);
    }
    else if (strcmp(node->name, "CALL") == 0 && node->child_count == 2) {
        // Procedure call (no return value)
        Node* func_name = node->children[0];
        Node* args = node->children[1];

        // Push parameters
        if (strcmp(args->name, "ARGS NONE") != 0) {
            for (int i = 0; i < args->child_count; i++) {
                char* arg = gen_expr(args->children[i]);
                printf("    PushParam %s\n", arg);
                free(arg);
            }
        }

        printf("    LCall %s\n", func_name->name);
        
        // Calculate and output PopParams size
        int param_size = calculate_param_size(args);
        if (param_size > 0) {
            printf("    PopParams %d\n", param_size);
        }
    }
    else if (strcmp(node->name, "BLOCK") == 0 || strcmp(node->name, "BODY") == 0) {
        for (int i = 0; i < node->child_count; i++) {
            gen_stmt(node->children[i]);
        }
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
        return strdup(node->name);
    }

    if (strcmp(node->name, "CALL") == 0 && node->child_count == 2) {
        // Function call that returns a value
        Node* func_name = node->children[0];
        Node* args = node->children[1];

        // Push parameters
        if (strcmp(args->name, "ARGS NONE") != 0) {
            for (int i = 0; i < args->child_count; i++) {
                char* arg = gen_expr(args->children[i]);
                printf("    PushParam %s\n", arg);
                free(arg);
            }
        }

        char* temp = new_temp();
        printf("    %s = LCall %s\n", temp, func_name->name);
        
        // Calculate and output PopParams size
        int param_size = calculate_param_size(args);
        if (param_size > 0) {
            printf("    PopParams %d\n", param_size);
        }
        
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
// Fixed version of count_local_vars and gen_func
int count_local_vars(Node* decls) {
    if (!decls) return 0;
    int count = 0;

    printf("DEBUG: count_local_vars - node name: '%s', child_count: %d\n", decls->name, decls->child_count);

    if (strcmp(decls->name, "VAR_LIST") == 0) {
        // VAR_LIST contains individual variable declarations
        count = decls->child_count;
        printf("DEBUG: VAR_LIST has %d children\n", count);
    } 
    else if (strcmp(decls->name, "BLOCK") == 0) {
        // BLOCK might contain VAR_LIST as children
        for (int i = 0; i < decls->child_count; i++) {
            if (strcmp(decls->children[i]->name, "VAR_LIST") == 0) {
                count += decls->children[i]->child_count;
                printf("DEBUG: Found VAR_LIST in BLOCK with %d variables\n", decls->children[i]->child_count);
            } else {
                // Recursively count variables in nested blocks
                count += count_local_vars(decls->children[i]);
            }
        }
    } 
    else if (decls->child_count == 0) {
        // Single variable declaration
        count = 1;
        printf("DEBUG: Single variable declaration\n");
    }

    printf("DEBUG: Total variable count: %d\n", count);
    return count;
}

void gen_func(Node* node) {
    if (!node || node->child_count < 3) return;

    const char* func_name = node->name;
    
    if (strcmp(func_name, "_main_") == 0) {
        printf("main:\n");
    } else {
        printf("%s:\n", func_name);
    }
    
    Node* params = node->children[0];
    Node* ret_type = node->children[1];
    Node* body = node->children[2];

    // Calculate memory for BOTH parameters AND local variables
    int param_size = calculate_param_var_size(params);
    
    int local_var_size = 0;
    if (body && strcmp(body->name, "BODY") == 0 && body->child_count >= 1) {
        Node* decls = body->children[0];
        local_var_size = calculate_local_var_size(decls);
    }

    // Total memory = parameters + local variables + overhead
    // Different overhead for main vs other functions
    int overhead = (strcmp(func_name, "_main_") == 0) ? 20 : 8;
    int total_size = param_size + local_var_size + overhead;

    printf("    BeginFunc %d\n", total_size);

    Node* stmts = body->child_count == 2 ? body->children[1] : body;
    gen_stmt(stmts);

    printf("    EndFunc\n");
}

int calculate_local_var_size(Node* decls) {
    if (!decls) return 0;
    int total_size = 0;

    if (strcmp(decls->name, "VAR_LIST") == 0) {
        // Process each variable declaration
        for (int i = 0; i < decls->child_count; i++) {
            Node* var_decl = decls->children[i];
            if (var_decl->child_count == 0) {
                // Parse "TYPE varname" format
                char* copy = strdup(var_decl->name);
                char* type = strtok(copy, " ");
                if (type) {
                    total_size += size_of_type(type);
                }
                free(copy);
            } else {
                // Handle complex variable declarations recursively
                total_size += calculate_local_var_size(var_decl);
            }
        }
    } 
    else if (strcmp(decls->name, "BLOCK") == 0) {
        // Process nested blocks
        for (int i = 0; i < decls->child_count; i++) {
            total_size += calculate_local_var_size(decls->children[i]);
        }
    }
    else if (decls->child_count == 0) {
        // Single variable declaration
        char* copy = strdup(decls->name);
        char* type = strtok(copy, " ");
        if (type) {
            total_size += size_of_type(type);
        }
        free(copy);
    }

    return total_size;
}

int calculate_param_size(Node* args) {
    if (!args || strcmp(args->name, "ARGS NONE") == 0) return 0;
    
    int total_size = 0;
    for (int i = 0; i < args->child_count; i++) {
        // For simplicity, assume all parameters are 4 bytes
        // In a more complex system, you'd determine actual type sizes
        total_size += 4;
    }
    return total_size;
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

