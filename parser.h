#ifndef PARSER_H
#define PARSER_H

typedef struct Node {
    char* name;
    int child_count;
    struct Node** children;
} Node;

// Structure to track function information
typedef struct {
    char* name;
    int param_count;
    char** param_types;  // Array of parameter types
} FunctionInfo;

typedef struct {
    char* name;
    char* type;
} VarInfo;


// Function declarations
Node* create_node(char* name, int child_count, ...);
void print_ast(Node* node, int depth);
void free_ast(Node* node);

// New function declarations for semantic checks
void add_function(char* name, int param_count, Node* params);
int is_function_defined(const char* name);
int get_function_param_count(const char* name);
void check_param_count(const char* func_name, int provided_count, int line);
void check_param_types(const char* func_name, Node* args_node, int line);
int check_function_call(const char* name, int line);
void check_var_use(const char* name, int line);
char* get_var_type(const char* name);
char* get_expr_type(Node* expr);

#endif
