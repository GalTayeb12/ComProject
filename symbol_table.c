#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

#define MAX_SCOPE 1000

static Symbol* table = NULL;
static int current_scope = 0;
extern int main_defined;

extern int yylineno;

void init_symbol_table() {
    table = NULL;
    current_scope = 0;
}

void enter_scope() {
    current_scope++;
}

void exit_scope() {
    Symbol **curr = &table;
    while (*curr) {
        if ((*curr)->scope == current_scope) {
            Symbol *temp = *curr;
            *curr = (*curr)->next;
            free(temp->name);
            free(temp);
        } else {
            curr = &((*curr)->next);
        }
    }
    current_scope--;
}

int insert_symbol(const char *name, SymbolKind kind, VarType type, int param_index) {
    Symbol *s = lookup_symbol(name);
    if (s && s->scope == current_scope) {
        return 0; // קיים כבר באותו scope
    }
    if (strcmp(name, "_main_") == 0) {
    if (main_defined) {
        semantic_error("Function _main_ already defined", yylineno);
        return 0;
    }
    if (type != TYPE_VOID || param_index != -1) {
        semantic_error("_main_ must have void return type and no parameters", yylineno);
        return 0;
    }
    main_defined = 1;
}


    Symbol *new_sym = (Symbol*)malloc(sizeof(Symbol));
    new_sym->name = strdup(name);
    new_sym->kind = kind;
    new_sym->type = type;
    new_sym->scope = current_scope;
    new_sym->param_index = param_index;
    new_sym->next = table;
    table = new_sym;
    return 1;
}

Symbol* lookup_symbol(const char *name) {
    Symbol *curr = table;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void semantic_error(const char *msg, int yylineno) {
    fprintf(stderr, "Semantic error at line %d: %s\n", yylineno, msg);
}

void print_symbols() {
    Symbol *curr = table;
    printf("Symbol Table:\n");
    while (curr) {
        printf("Name: %s, Kind: %d, Type: %d, Scope: %d\n", curr->name, curr->kind, curr->type, curr->scope);
        curr = curr->next;
    }
}

