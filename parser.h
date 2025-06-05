#ifndef PARSER_H
#define PARSER_H

typedef struct Node {
    char* name;
    int child_count;
    struct Node** children;
} Node;

typedef struct {
    char* name;
    int param_count;
    char** param_types;
    char* return_type;
} FunctionInfo;

typedef struct {
    char* name;
    char* type;
} VarInfo;

Node* create_node(char* name, int child_count, ...);
void print_ast(Node* node, int depth);
void free_ast(Node* node);

// Semantic checks
void add_function(char* name, int param_count, Node* params, char* ret_type);
int is_function_defined(const char* name);
int get_function_param_count(const char* name);
void check_param_count(const char* func_name, int provided_count, int line);
void check_param_types(const char* func_name, Node* args_node, int line);
int check_function_call(const char* name, int line);
void check_var_use(const char* name, int line);
char* get_var_type(const char* name);
char* get_expr_type(Node* expr);

// 3AC functions
void gen_code(Node* root);
char* new_temp();
char* new_label();
int is_procedure(const char* func_name);

#endif

