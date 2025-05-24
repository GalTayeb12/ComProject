
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

typedef enum {
    TYPE_INT, TYPE_REAL, TYPE_CHAR, TYPE_BOOL, TYPE_STRING,
    TYPE_INT_PTR, TYPE_REAL_PTR, TYPE_CHAR_PTR,
    TYPE_NONE, // For void return types (procedures)
    TYPE_INVALID
} VarType;

typedef enum {
    SYM_VAR,
    SYM_FUNC
} SymbolKind;

typedef struct Symbol {
    char* name;
    VarType type;
    SymbolKind kind;
    int param_count;
    VarType* param_types;
    int scope_level;
    struct Symbol* next;
} Symbol;

typedef struct Scope {
    Symbol* symbols;
    struct Scope* next;
} Scope;

// Scope management
void push_scope();
void pop_scope();
void reset_symbol_table();

// Symbol management
bool insert_symbol(char* name, VarType type, SymbolKind kind, int param_count, VarType* param_types);
Symbol* lookup_symbol(char* name);
Symbol* lookup_symbol_current_scope(char* name);

// Main function validity check
bool is_main_defined();
bool is_main_valid();  // no args, no return type

// Utility
void print_symbol_table(); // optional, for debugging
void semantic_error(const char* msg, int lineno);

#endif

