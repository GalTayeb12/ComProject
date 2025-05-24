#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int temp_id = 0;
int label_id = 1;

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
                char* copy = strdup(child->name);
                char* type = strtok(copy, " ");
                total += size_of_type(type);
                free(copy);
            } else {
                for (int j = 0; j < child->child_count; j++) {
                    char* copy = strdup(child->children[j]->name);
                    char* type = strtok(copy, " ");
                    total += size_of_type(type);
                    free(copy);
                }
            }
        }
    } else if (decls->child_count == 0) {
        char* copy = strdup(decls->name);
        char* type = strtok(copy, " ");
        total += size_of_type(type);
        free(copy);
    }

    return total;
}

char* gen_expr(Node* node);

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
        char* cond = gen_expr(node->children[0]);
        char* label_true = new_label();
        char* label_false = new_label();
        char* label_end = new_label();

        printf("    if %s Goto %s\n", cond, label_true);
        printf("    Goto %s\n", label_false);
        printf("%s:\n", label_true);
        gen_stmt(node->children[1]);
        printf("    Goto %s\n", label_end);
        printf("%s:\n", label_false);
        gen_stmt(node->children[2]);
        printf("%s:\n", label_end);

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
        printf("%s:\n", label_body);
        gen_stmt(node->children[1]);
        printf("    Goto %s\n", label_start);
        printf("%s:\n", label_end);

        free(cond); free(label_start); free(label_body); free(label_end);
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
        Node* func_name = node->children[0];
        Node* args = node->children[1];
        int arg_count = args->child_count;

        for (int i = 0; i < arg_count; i++) {
            char* arg = gen_expr(args->children[i]);
            printf("    PushParam %s\n", arg);
            free(arg);
        }

        char* temp = new_temp();
        printf("    %s = LCall %s\n", temp, func_name->name);
        printf("    PopParams %d\n", arg_count);
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

    // חשב משתנים מקומיים
    int local_count = 0;
    if (body && strcmp(body->name, "BODY") == 0 && body->child_count >= 1) {
        Node* decls = body->children[0];
        if (strcmp(decls->name, "VAR_LIST") == 0 || strcmp(decls->name, "BLOCK") == 0) {
            local_count = decls->child_count;
        } else if (decls->child_count == 0) {
            local_count = 1;
        }
    }

    printf("%s:\n", func_name); // ← עכשיו זה מדפיס את השם האמיתי
    printf("    BeginFunc %d\n", local_count * 4); // אם INT בגודל 4

    Node* stmts = body->child_count == 2 ? body->children[1] : body;
    gen_stmt(stmts);

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

