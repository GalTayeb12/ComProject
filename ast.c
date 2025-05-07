#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"

extern int main_defined;


Node* create_node(const char* name, int num_children, ...) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->name = strdup(name);
    node->num_children = num_children;

    if (num_children > 0) {
        node->children = (Node**)malloc(sizeof(Node*) * num_children);

        va_list args;
        va_start(args, num_children);
        for (int i = 0; i < num_children; i++) {
            node->children[i] = va_arg(args, Node*);
        }
        va_end(args);
    } else {
        node->children = NULL;
    }

    return node;
}

void print_function_node(Node* node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("%s\n", node->name);

    for (int i = 0; i < node->num_children; i++) {
        print_ast(node->children[i], depth + 1);
    }
}

void print_functions(Node* func_node, int depth) {
    if (!func_node || func_node->num_children == 0)
        return;

    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("(FUNC\n");

    print_function_node(func_node->children[0], depth + 1);

    for (int i = 0; i < depth; i++)
        printf("  ");
    printf(")\n");

    if (func_node->num_children >= 2) {
        if (strcmp(func_node->children[1]->name, "FUNC") == 0) {
            print_functions(func_node->children[1], depth);
        } else {
            for (int i = 0; i < depth; i++)
                printf("  ");
            printf("(FUNC\n");
            print_function_node(func_node->children[1], depth + 1);
            for (int i = 0; i < depth; i++)
                printf("  ");
            printf(")\n");
        }
    }
}

void print_ast(Node* node, int depth) {
    if (!node)
        return;

    if (strcmp(node->name, "BLOCK") == 0 && node->num_children == 0)
        return;

    if (strcmp(node->name, "CODE") == 0) {
        printf("(CODE\n");

        for (int i = 0; i < node->num_children; i++) {
            if (strcmp(node->children[i]->name, "FUNC") == 0) {
                print_functions(node->children[i], 1);
            } else {
                for (int j = 0; j < 1; j++)
                    printf("  ");
                printf("(FUNC\n");
                print_function_node(node->children[i], 2);
                for (int j = 0; j < 1; j++)
                    printf("  ");
                printf(")\n");
            }
        }

        printf(")\n");
        return;
    }

    if (node->num_children == 2 && (
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
        printf("(%s\n", node->name);
        for (int i = 0; i < node->num_children; i++) {
            print_ast(node->children[i], depth + 1);
        }
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf(")\n");
        return;
    }

    if (node->num_children == 2 && strcmp(node->name, "=") == 0 && node->children[0]->num_children == 0) {
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("(= %s\n", node->children[0]->name);
        print_ast(node->children[1], depth + 1);
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf(")\n");
        return;
    }

    if (node->num_children == 1 && node->children[0]->num_children == 0 &&
        strcmp(node->name, "RET") == 0) {
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("(RET %s)\n", node->children[0]->name);
        return;
    }

    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("(%s\n", node->name);

    for (int i = 0; i < node->num_children; i++) {
        print_ast(node->children[i], depth + 1);
    }

    for (int i = 0; i < depth; i++)
        printf("  ");
    printf(")\n");
}

