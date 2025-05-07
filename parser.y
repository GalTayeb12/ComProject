%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"

int yylex(void);
int yyerror(const char* s) {
    fprintf(stderr, "Syntax error: %s\n", s);
    return 1;
}

Node* root;
int main_defined = 0;

%}

%locations
%initial-action
{
    init_symbol_table();
}

%union {
    Node* node;
    char* str;
}

%token <str> IDENTIFIER
%token <str> TYPE
%token <str> STRING_LITERAL CHAR_LITERAL BOOL_LITERAL INT_LITERAL REAL_LITERAL

%token DEF RETURNS RETURN IF ELSE ELIF WHILE FOR DO VAR BEGIN_T END_T CALL NULL_T TRUE FALSE VOID
%token INT REAL CHAR BOOL STRING INT_PTR REAL_PTR CHAR_PTR
%token ADD SUB MUL DIV ASSIGN EQ NEQ LEQ GEQ LT GT AND OR NOT ADDR PIPE_SYMBOL

%right ASSIGN
%left OR
%left AND
%left EQ NEQ
%left LT LEQ GT GEQ
%left ADD SUB
%left MUL DIV
%right UMINUS

%type <node> program funcs func func_header parameters parameter ret_type type_name block body inner_block stmts stmt call_stmt for_expr optional_var_list var_decl_list var_single_decl expr args

%%

program
    : funcs { root = create_node("CODE", 1, $1); }
    ;

funcs
    : func { $$ = $1; }
    | func funcs { $$ = create_node("FUNC", 2, $1, $2); }
    | /* empty */ { $$ = NULL; }
    ;

func
    : func_header body {
        enter_scope();  // התחלת גוף הפונקציה
        $$ = create_node($1->name, 3, $1->children[0], $1->children[1], create_node("BODY", 1, $2));
        exit_scope();   // סיום גוף הפונקציה
    }
;


func_header
    : DEF IDENTIFIER '(' parameters ')' ':' RETURNS ret_type {
        $$ = create_node($2, 2, $4, $8);

        VarType ret;
        if (strcmp($8->name, "INT") == 0) ret = TYPE_INT;
        else if (strcmp($8->name, "REAL") == 0) ret = TYPE_REAL;
        else if (strcmp($8->name, "CHAR") == 0) ret = TYPE_CHAR;
        else if (strcmp($8->name, "BOOL") == 0) ret = TYPE_BOOL;
        else if (strcmp($8->name, "INT_PTR") == 0) ret = TYPE_INT_PTR;
        else if (strcmp($8->name, "REAL_PTR") == 0) ret = TYPE_REAL_PTR;
        else if (strcmp($8->name, "CHAR_PTR") == 0) ret = TYPE_CHAR_PTR;
        else ret = TYPE_VOID;

        if (!insert_symbol($2, FUNC_SYM, ret, -1)) {
            semantic_error("Function already defined in this scope", @2.first_line);
            YYABORT;
        }

        if (strcmp($2, "_main_") == 0) {
            if (main_defined) {
                semantic_error("Multiple definitions of _main_", @2.first_line);
                YYABORT;
            }

            if (ret != TYPE_VOID) {
                semantic_error("_main_ must not return a value", @2.first_line);
                YYABORT;
            }

            if ($4->num_children > 0) {
                semantic_error("_main_ must not have parameters", @2.first_line);
                YYABORT;
            }

            main_defined = 1;  // ✅ אם כל התנאים עברו
        }
    }

    | DEF IDENTIFIER '(' ')' ':' RETURNS ret_type {
        $$ = create_node($2, 2, create_node("PARS", 0), $7);

        VarType ret;
        if (strcmp($7->name, "INT") == 0) ret = TYPE_INT;
        else if (strcmp($7->name, "REAL") == 0) ret = TYPE_REAL;
        else if (strcmp($7->name, "CHAR") == 0) ret = TYPE_CHAR;
        else if (strcmp($7->name, "BOOL") == 0) ret = TYPE_BOOL;
        else if (strcmp($7->name, "INT_PTR") == 0) ret = TYPE_INT_PTR;
        else if (strcmp($7->name, "REAL_PTR") == 0) ret = TYPE_REAL_PTR;
        else if (strcmp($7->name, "CHAR_PTR") == 0) ret = TYPE_CHAR_PTR;
        else ret = TYPE_VOID;

        if (!insert_symbol($2, FUNC_SYM, ret, -1)) {
            semantic_error("Function already defined in this scope", @2.first_line);
            YYABORT;
        }

        if (strcmp($2, "_main_") == 0) {
            if (main_defined) {
                semantic_error("Multiple definitions of _main_", @2.first_line);
                YYABORT;
            }

            if (ret != TYPE_VOID) {
                semantic_error("_main_ must not return a value", @2.first_line);
                YYABORT;
            }

            main_defined = 1;
        }
    }
;

parameters
    : parameter                                { $$ = create_node("PARS", 1, $1); }
    | parameter ',' parameters                { $$ = create_node("PARS", 2, $1, $3); }
    ;

parameter
    : type_name ':' IDENTIFIER                {
        char temp[128];
        sprintf(temp, "%s:%s", $1->name, $3);
        $$ = create_node(strdup(temp), 0);
    }
    ;


ret_type
    : type_name { $$ = $1; }
    ;

type_name
    : INT       { $$ = create_node("INT", 0); }
    | REAL      { $$ = create_node("REAL", 0); }
    | CHAR      { $$ = create_node("CHAR", 0); }
    | BOOL      { $$ = create_node("BOOL", 0); }
    | INT_PTR   { $$ = create_node("INT_PTR", 0); }
    | REAL_PTR  { $$ = create_node("REAL_PTR", 0); }
    | CHAR_PTR  { $$ = create_node("CHAR_PTR", 0); }
    | VOID      { $$ = create_node("VOID", 0); }
    ;

body
    : VAR optional_var_list block { $$ = create_node("VAR_BLOCK", 2, $2, $3); }
    | block { $$ = $1; }
    ;

block
    : BEGIN_T inner_block END_T {
        enter_scope();
        $$ = $2;
        exit_scope();
    }
;


inner_block
    : stmts { $$ = create_node("BLOCK", 1, $1); }
    | /* empty */ { $$ = create_node("BLOCK", 0); }
    ;

stmts
    : stmt { $$ = $1; }
    | stmt stmts { $$ = create_node("BLOCK", 2, $1, $2); }
    ;

stmt
    : expr ';'                                              { $$ = $1; }
    | RETURN expr ';'                                       { $$ = create_node("RETURN", 1, $2); }

    // IF only
    | IF expr ':' block                                     { $$ = create_node("IF", 2, $2, $4); }

    // IF + ELSE
    | IF expr ':' block ELSE ':' block                      { $$ = create_node("IF_ELSE", 3, $2, $4, $7); }

    // IF + ELIF
    | IF expr ':' block ELIF expr ':' block                 { 
        $$ = create_node("IF_ELIF", 3, $2, $4, create_node("ELIF", 2, $6, $8)); 
    }

    // IF + ELIF + ELSE
    | IF expr ':' block ELIF expr ':' block ELSE ':' block {
        $$ = create_node("IF_ELIF_ELSE", 4, $2, $4, create_node("ELIF", 2, $6, $8), $11);
    }

    // WHILE
    | WHILE expr ':' block          { $$ = create_node("WHILE", 2, $2, $4); }

    // FOR
    | FOR '(' for_expr ';' for_expr ';' for_expr ')' ':' block {
        $$ = create_node("FOR", 4, $3, $5, $7, $10);
    }

    // CALL
    | call_stmt ';'    { $$ = $1; }
    ;


call_stmt
    : CALL IDENTIFIER '(' args ')' {
        $$ = create_node("CALL", 2, create_node($2, 0), $4);
    }
    ;

for_expr
    : expr { $$ = $1; }
    | /* empty */ { $$ = create_node("EMPTY", 0); }
    ;

optional_var_list
    : var_decl_list { $$ = $1; }
    | /* empty */ { $$ = create_node("BLOCK", 0); }
    ;

var_decl_list
    : var_single_decl { $$ = create_node("BLOCK", 1, $1); }
    | var_decl_list var_single_decl { $$ = create_node("BLOCK", 2, $1, $2); }
    ;

var_single_decl
    : TYPE IDENTIFIER ASSIGN expr ';' {
        // צורה: type int:x = 5;
        char* full = $2;
        char* colon = strchr(full, ':');
        if (!colon) {
            semantic_error("Invalid format in variable declaration, expected format: type int:x", @2.first_line);
            YYABORT;
        }

        *colon = '\0'; // now full = "int", colon+1 = "x"
        char* typename = full;
        char* varname = colon + 1;

        VarType t;
        if (strcmp(typename, "int") == 0) t = TYPE_INT;
        else if (strcmp(typename, "real") == 0) t = TYPE_REAL;
        else if (strcmp(typename, "char") == 0) t = TYPE_CHAR;
        else if (strcmp(typename, "bool") == 0) t = TYPE_BOOL;
        else if (strcmp(typename, "int*") == 0) t = TYPE_INT_PTR;
        else if (strcmp(typename, "real*") == 0) t = TYPE_REAL_PTR;
        else if (strcmp(typename, "char*") == 0) t = TYPE_CHAR_PTR;
        else t = TYPE_VOID;

        if (!insert_symbol(varname, VAR_SYM, t, -1)) {
            semantic_error("Variable already declared in this scope", @2.first_line);
            YYABORT;
        }

        Node* var_node = create_node(varname, 0);
        $$ = create_node("=", 2, var_node, $4);
    }

    | TYPE type_name ':' IDENTIFIER ASSIGN expr ';' {
        // צורה: type INT : x = 5;
        VarType t;
        if (strcmp($2->name, "INT") == 0) t = TYPE_INT;
        else if (strcmp($2->name, "REAL") == 0) t = TYPE_REAL;
        else if (strcmp($2->name, "CHAR") == 0) t = TYPE_CHAR;
        else if (strcmp($2->name, "BOOL") == 0) t = TYPE_BOOL;
        else if (strcmp($2->name, "INT_PTR") == 0) t = TYPE_INT_PTR;
        else if (strcmp($2->name, "REAL_PTR") == 0) t = TYPE_REAL_PTR;
        else if (strcmp($2->name, "CHAR_PTR") == 0) t = TYPE_CHAR_PTR;
        else t = TYPE_VOID;

        if (!insert_symbol($4, VAR_SYM, t, -1)) {
            semantic_error("Variable already declared in this scope", @4.first_line);
            YYABORT;
        }

        Node* var_node = create_node($4, 0);
        $$ = create_node("=", 2, var_node, $6);
    }
    ;

args
    : expr { $$ = create_node("ARGS", 1, $1); }
    | expr ',' args { $$ = create_node("ARGS", 2, $1, $3); }
    | /* empty */ { $$ = create_node("ARGS NONE", 0); }
    ;

expr
    : expr ASSIGN expr    { $$ = create_node("=", 2, $1, $3); }  // תמיכה ב־x = 1
    | expr ADD expr       { $$ = create_node("+", 2, $1, $3); }
    | expr SUB expr       { $$ = create_node("-", 2, $1, $3); }
    | expr MUL expr       { $$ = create_node("*", 2, $1, $3); }
    | expr DIV expr       { $$ = create_node("/", 2, $1, $3); }
    | SUB expr %prec UMINUS { $$ = create_node("-", 2, create_node("0", 0), $2); }
    | expr EQ expr        { $$ = create_node("==", 2, $1, $3); }
    | expr NEQ expr       { $$ = create_node("!=", 2, $1, $3); }
    | expr LEQ expr       { $$ = create_node("<=", 2, $1, $3); }
    | expr GEQ expr       { $$ = create_node(">=", 2, $1, $3); }
    | expr LT expr        { $$ = create_node("<", 2, $1, $3); }
    | expr GT expr        { $$ = create_node(">", 2, $1, $3); }
    | '(' expr ')'        { $$ = $2; }
    | IDENTIFIER          { $$ = create_node($1, 0); }
    | INT_LITERAL         { $$ = create_node($1, 0); }
    | REAL_LITERAL        { $$ = create_node($1, 0); }
    | CHAR_LITERAL        { $$ = create_node($1, 0); }
    | STRING_LITERAL      { $$ = create_node($1, 0); }
    | TRUE                { $$ = create_node("TRUE", 0); }
    | FALSE               { $$ = create_node("FALSE", 0); }
    ;

%%


int main() {
    if (yyparse() == 0 && root != NULL) {
        print_ast(root, 0);
    }
    return 0;
}



