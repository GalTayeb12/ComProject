#ifndef AST_H
#define AST_H

// מבנה לעץ תחביר אבסטרקטי (AST)
typedef struct Node {
    char* name;
    int num_children;
    struct Node** children;
} Node;

// פונקציה ליצירת צומת חדש בעץ
Node* create_node(const char* name, int num_children, ...);

// פונקציה להדפסת העץ (לדוגמה: preorder)
void print_ast(Node* node, int depth);

#endif // AST_H

