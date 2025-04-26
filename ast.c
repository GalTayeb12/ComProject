#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "parser.h"

Node* create_node(char* name, int child_count, ...) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->name = strdup(name);
    node->child_count = child_count;
    
    if (child_count > 0) {
        node->children = (Node**)malloc(sizeof(Node*) * child_count);
        
        va_list args;
        va_start(args, child_count);
        for (int i = 0; i < child_count; i++) {
            node->children[i] = va_arg(args, Node*);
        }
        va_end(args);
    } else {
        node->children = NULL;
    }

    return node;
}

// Function to print a function node without parentheses around the function name
void print_function_node(Node* node, int depth) {
    if (!node) return;
    
    // Print the function name without parentheses
    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("%s\n", node->name);
    
    // Print all children
    for (int i = 0; i < node->child_count; i++) {
        print_ast(node->children[i], depth + 1);
    }
}

// Function to extract functions from the FUNC node
void print_functions(Node* func_node, int depth) {
    if (!func_node || func_node->child_count == 0)
        return;
    
    // Print the first function
    if (func_node->child_count >= 1) {
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("(FUNC\n");
        
        // Print the function name WITHOUT parentheses and its content
        print_function_node(func_node->children[0], depth + 1);
        
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf(")\n");
    }
    
    // Check if there's a second child that's a FUNC node
    if (func_node->child_count >= 2 && strcmp(func_node->children[1]->name, "FUNC") == 0) {
        print_functions(func_node->children[1], depth);
    }
    // If second child is a regular function
    else if (func_node->child_count >= 2) {
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("(FUNC\n");
        
        print_function_node(func_node->children[1], depth + 1);
        
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf(")\n");
    }
}

void print_ast(Node* node, int depth) {
    if (!node)
        return;
        
    if (strcmp(node->name, "BLOCK") == 0 && node->child_count == 0)
        return;

    
    if (strcmp(node->name, "CODE") == 0) {
        printf("(CODE\n");
        
        if (node->child_count > 0 && strcmp(node->children[0]->name, "FUNC") == 0) {
            print_functions(node->children[0], 1);
        } else {
            for (int i = 0; i < node->child_count; i++) {
                print_ast(node->children[i], depth + 1);
            }
        }
        
        printf(")\n");
        return;
    }
    
    if (node->child_count == 2 && (
        strcmp(node->name, "+") == 0 || 
        strcmp(node->name, "-") == 0 ||
        strcmp(node->name, "*") == 0 ||
        strcmp(node->name, "/") == 0 ||
        strcmp(node->name, ">") == 0 ||
        strcmp(node->name, "<") == 0 ||
        strcmp(node->name, ">=") == 0 ||
        strcmp(node->name, "<=") == 0 ||
        strcmp(node->name, "==") == 0 ||
        strcmp(node->name, "!=") == 0 ||
        strcmp(node->name, "AND") == 0 ||
        strcmp(node->name, "OR") == 0)) {
        
        for (int i = 0; i < depth; i++)
            printf("  ");
        
        if (node->children[0]->child_count == 0 && node->children[1]->child_count == 0) {
            printf("(%s %s %s)\n", node->name, node->children[0]->name, node->children[1]->name);
        } else if (node->children[1]->child_count == 0) {
            printf("(%s\n", node->name);
            print_ast(node->children[0], depth + 1);
            for (int i = 0; i < depth + 1; i++)
                printf("  ");
            printf("%s\n", node->children[1]->name);
            for (int i = 0; i < depth; i++)
                printf("  ");
            printf(")\n");
        } else {
            printf("(%s\n", node->name);
            for (int i = 0; i < node->child_count; i++) {
                print_ast(node->children[i], depth + 1);
            }
            for (int i = 0; i < depth; i++)
                printf("  ");
            printf(")\n");
        }
        return;
    }
    
    if (node->child_count == 2 && strcmp(node->name, "=") == 0 && node->children[0]->child_count == 0) {
        for (int i = 0; i < depth; i++)
            printf("  ");
        
        printf("(= %s\n", node->children[0]->name);
        print_ast(node->children[1], depth + 1);
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf(")\n");
        return;
    }
    
    if (node->child_count == 1 && node->children[0]->child_count == 0 && 
        strcmp(node->name, "RET") == 0) {
        for (int i = 0; i < depth; i++)
            printf("  ");
        
        printf("(RET %s)\n", node->children[0]->name);
        return;
    }
    
    for (int i = 0; i < depth; i++)
        printf("  ");
    
    printf("(%s", node->name);
    
    if (node->child_count == 0) {
        printf(")\n");
        return;
    }
    
    printf("\n");
    
    // Print all children
    for (int i = 0; i < node->child_count; i++) {
        print_ast(node->children[i], depth + 1);
    }
    
    for (int i = 0; i < depth; i++)
        printf("  ");
    
    printf(")\n");
}
