#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef enum { VAR_SYM, FUNC_SYM, PARAM_SYM } SymbolKind;
typedef enum { TYPE_INT, TYPE_REAL, TYPE_CHAR, TYPE_BOOL, TYPE_STRING, TYPE_INT_PTR, TYPE_REAL_PTR, TYPE_CHAR_PTR, TYPE_VOID } VarType;

typedef struct Symbol {
    char *name;
    SymbolKind kind;
    VarType type;
    int scope;
    int param_index; // עבור פרמטרים
    struct Symbol *next;
} Symbol;

extern int main_defined;

void init_symbol_table();
void enter_scope();
void exit_scope();
int insert_symbol(const char *name, SymbolKind kind, VarType type, int param_index);
Symbol* lookup_symbol(const char *name);
void print_symbols();
void semantic_error(const char *msg, int lineno);

#endif

