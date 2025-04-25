#ifndef PARSER_H
#define PARSER_H

typedef struct Node {
    char* name;
    int child_count;
    struct Node** children;
} Node;

Node* create_node(char* name, int child_count, ...);
void print_ast(Node* node, int depth);
void free_ast(Node* node);

#endif
