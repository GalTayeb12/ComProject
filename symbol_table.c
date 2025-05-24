#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

static Scope* current_scope = NULL;
static int current_scope_level = 0;

static int main_defined = 0;
static int main_valid = 0;

// ================= Scope Management =================

void push_scope() {
    Scope* new_scope = (Scope*)malloc(sizeof(Scope));
    new_scope->symbols = NULL;
    new_scope->next = current_scope;
    current_scope = new_scope;
    current_scope_level++;
}

void pop_scope() {
    if (!current_scope) return;
    Symbol* sym = current_scope->symbols;
    while (sym) {
        Symbol* tmp = sym;
        sym = sym->next;
        if (tmp->param_types) free(tmp->param_types);
        free(tmp->name);
        free(tmp);
    }
    Scope* tmp_scope = current_scope;
    current_scope = current_scope->next;
    free(tmp_scope);
    current_scope_level--;
}

void reset_symbol_table() {
    while (current_scope) pop_scope();
    main_defined = 0;
    main_valid = 0;
}

// ================= Symbol Management =================

bool insert_symbol(char* name, VarType type, SymbolKind kind, int param_count, VarType* param_types) {
    if (!current_scope) push_scope();

    if (lookup_symbol_current_scope(name)) {
        return false;  // duplicate in same scope
    }

    Symbol* new_sym = (Symbol*)malloc(sizeof(Symbol));
    new_sym->name = strdup(name);
    new_sym->type = type;
    new_sym->kind = kind;
    new_sym->param_count = param_count;
    new_sym->param_types = NULL;
    if (param_count > 0 && param_types) {
        new_sym->param_types = (VarType*)malloc(sizeof(VarType) * param_count);
        for (int i = 0; i < param_count; i++) {
            new_sym->param_types[i] = param_types[i];
        }
    }
    new_sym->scope_level = current_scope_level;
    new_sym->next = current_scope->symbols;
    current_scope->symbols = new_sym;

    if (strcmp(name, "_main_") == 0 && kind == SYM_FUNC) {
        main_defined = 1;
        if (param_count == 0 && type == TYPE_NONE) {
            main_valid = 1;
        }
    }

    return true;
}

Symbol* lookup_symbol_current_scope(char* name) {
    if (!current_scope) return NULL;
    Symbol* sym = current_scope->symbols;
    while (sym) {
        if (strcmp(sym->name, name) == 0) return sym;
        sym = sym->next;
    }
    return NULL;
}

Symbol* lookup_symbol(char* name) {
    Scope* scope = current_scope;
    while (scope) {
        Symbol* sym = scope->symbols;
        while (sym) {
            if (strcmp(sym->name, name) == 0) return sym;
            sym = sym->next;
        }
        scope = scope->next;
    }
    return NULL;
}

// ================= Main Function Checks =================

bool is_main_defined() {
    return main_defined;
}

bool is_main_valid() {
    return main_valid;
}

// ================= Utility =================

void semantic_error(const char* msg, int lineno) {
    fprintf(stderr, "Semantic error at line %d: %s\n", lineno, msg);
}

void print_symbol_table() {
    printf("===== SYMBOL TABLE =====\n");
    Scope* scope = current_scope;
    int level = current_scope_level;
    while (scope) {
        printf("Scope Level %d:\n", level--);
        Symbol* sym = scope->symbols;
        while (sym) {
            printf("  %s (%s)", sym->name, sym->kind == SYM_VAR ? "VAR" : "FUNC");
            if (sym->kind == SYM_FUNC) {
                printf(" returns %d, params: %d", sym->type, sym->param_count);
            } else {
                printf(" type: %d", sym->type);
            }
            printf("\n");
            sym = sym->next;
        }
        scope = scope->next;
    }
    printf("========================\n");
}

