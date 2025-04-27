%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

extern int yylineno;
extern int yylex();
void yyerror(const char* s);
extern char* yytext;

Node* root;

// Stack לניהול סוגי חזרה
#define MAX_DEPTH 100
char return_type_stack[MAX_DEPTH][20];
int stack_top = -1;
char current_return_type[20] = "";
char return_type_to_push[20] = "";
int current_func_has_return = 0;

// דגלים חכמים
int param_error = 0;
int comma_error = 0;
int return_type_error = 0;
int missing_return_error = 0;
int proc_return_error = 0;
int has_main = 0;

// טבלת פונקציות
#define MAX_FUNCTIONS 100
char* function_names[MAX_FUNCTIONS];
int function_count = 0;

// שמירת שמות פונקציות
void add_function(char* name) {
    if (function_count < MAX_FUNCTIONS)
        function_names[function_count++] = strdup(name);
}

// בדיקה אם פונקציה קיימת
int is_function_defined(const char* name) {
    for (int i = 0; i < function_count; i++) {
        if (strcmp(function_names[i], name) == 0)
            return 1;
    }
    return 0;
}

// מחסנית טיפוסי חזרה
void push_return_type(const char* type) {
    if (++stack_top < MAX_DEPTH) {
        strcpy(return_type_stack[stack_top], type);
        strcpy(current_return_type, type);
    }
}

void pop_return_type() {
    if (stack_top >= 0) {
        stack_top--;
        if (stack_top >= 0)
            strcpy(current_return_type, return_type_stack[stack_top]);
        else
            current_return_type[0] = '\0';
    }
}

// בדיקת התאמת סוג RETURN
int check_return_type(Node* expr_node) {
    if (!expr_node) return 1;

    if (current_return_type[0] == '\0') {
        proc_return_error = 1;
        return 0;
    }

    // קריאה לפונקציה תמיד חוקי
    if (strcmp(expr_node->name, "CALL") == 0)
        return 1;

    // משתנה
    if (expr_node->child_count == 0 && !isdigit(expr_node->name[0]) &&
        expr_node->name[0] != '\'' && expr_node->name[0] != '"' &&
        strcmp(expr_node->name, "TRUE") != 0 &&
        strcmp(expr_node->name, "FALSE") != 0 &&
        strcmp(expr_node->name, "NULL") != 0)
        return 1;

    if ((strcmp(expr_node->name, "TRUE") == 0 || strcmp(expr_node->name, "FALSE") == 0) &&
        strcmp(current_return_type, "BOOL") == 0)
        return 1;
    
    if (isdigit(expr_node->name[0]) && strcmp(current_return_type, "INT") == 0)
        return 1;
    
    if (strchr(expr_node->name, '.') && strcmp(current_return_type, "REAL") == 0)
        return 1;
    
    if (expr_node->name[0] == '\'' && strcmp(current_return_type, "CHAR") == 0)
        return 1;
    
    if (expr_node->name[0] == '"' && strcmp(current_return_type, "STRING") == 0)
        return 1;
    
    if (strcmp(expr_node->name, "NULL") == 0 &&
       (strcmp(current_return_type, "INT_PTR") == 0 ||
        strcmp(current_return_type, "REAL_PTR") == 0 ||
        strcmp(current_return_type, "CHAR_PTR") == 0))
        return 1;

    // אופרטורים בסיסיים
    if (strcmp(expr_node->name, "+") == 0 || strcmp(expr_node->name, "-") == 0 ||
        strcmp(expr_node->name, "*") == 0 || strcmp(expr_node->name, "/") == 0)
        return 1;

    return_type_error = 1;
    return 0;
}

// בדיקה אם פונקציה מסתיימת ב-RETURN
int ends_with_return(Node* node) {
    if (!node) return 0;
    if (strcmp(node->name, "BLOCK") == 0 || strcmp(node->name, "BODY") == 0) {
        if (node->child_count == 0) return 0;
        return ends_with_return(node->children[node->child_count - 1]);
    }
    if (strcmp(node->name, "RET") == 0) return 1;
    if (strcmp(node->name, "IF-ELSE") == 0 && node->child_count >= 3) {
        return ends_with_return(node->children[1]) && ends_with_return(node->children[2]);
    }
    return 0;
}
%}

%union {
    char* str;
    Node* node;
}

%token DEF RETURNS RETURN IF ELSE ELIF WHILE FOR DO VAR BEGIN_T END_T CALL NULL_T TRUE FALSE TYPE
%token INT REAL CHAR BOOL STRING INT_PTR REAL_PTR CHAR_PTR
%token AND OR NOT
%token EQ NE LE GE LT GT ASSIGN
%token ADD SUB MUL DIV ADDR DEREF PIPE_SYMBOL
%token <str> IDENTIFIER INT_LITERAL REAL_LITERAL CHAR_LITERAL STRING_LITERAL

%type <node> 
    program funcs func nested_func func_header 
    parameters parameter ret_type type
    block inner_block stmts stmt expr args
    var_decls optional_var_list var_decl_list var_single_decl var_init_list string_decl_list string_decl body

// קדימויות ואסוציאטיביות
%left OR
%left AND
%left EQ NE
%left LT LE GT GE
%left ADD SUB
%left MUL DIV
%right NOT ADDR
%right UMINUS
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE ELIF

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
  : func_header var_decls block {
      if (!ends_with_return($3) && strlen(current_return_type) > 0) {
          missing_return_error = 1;
          yyerror("Function with return type must end with a return statement");
      }
      $$ = create_node($1->name, 3, $1->children[0], $1->children[1], create_node("BODY", 2, $2, $3));
      pop_return_type();
  }
;

nested_func
  : func_header var_decls block {
      if (!ends_with_return($3) && strlen(current_return_type) > 0) {
          missing_return_error = 1;
          yyerror("Function with return type must end with a return statement");
      }
      $$ = create_node($1->name, 3, $1->children[0], $1->children[1], create_node("BODY", 2, $2, $3));
      pop_return_type();
  }
;

func_header
  : DEF IDENTIFIER '(' parameters ')' ':' RETURNS ret_type {
      push_return_type(return_type_to_push);
      Node* params = $4;
      Node* ret_type_node = $8;
      if (strcmp($2, "_main_") == 0)
          yyerror("Error: _main_() cannot return a value");
      add_function($2);
      $$ = create_node($2, 2, params, ret_type_node);
  }
  | DEF IDENTIFIER '(' parameters ')' ':' {
      push_return_type(""); 
      Node* params = $4;
      Node* ret_type_node = create_node("RET NONE", 0);
      if (strcmp($2, "_main_") == 0)
          has_main = 1;
      add_function($2);
      $$ = create_node($2, 2, params, ret_type_node);
  }
;

parameters
  : parameter ';' parameters {
      Node* pars = create_node("PARS", $3->child_count + 1);
      pars->children[0] = $1;
      for (int i = 0; i < $3->child_count; i++)
          pars->children[i+1] = $3->children[i];
      $$ = pars;
  }
  | parameter {
      $$ = create_node("PARS", 1, $1);
  }
  | /* empty */ {
      $$ = create_node("PARS NONE", 0);
  }
;

parameter
  : IDENTIFIER type ':' IDENTIFIER {
      char temp[100];
      sprintf(temp, "par%s %s %s", $1 + 3, $2->name, $4);
      $$ = create_node(temp, 0);
  }
  | IDENTIFIER ':' IDENTIFIER {
      param_error = 1;
      yyerror("no type defined");
      $$ = create_node("ERROR", 0);
  }
;

ret_type
  : INT { strcpy(return_type_to_push, "INT"); $$ = create_node("INT", 0); }
  | REAL { strcpy(return_type_to_push, "REAL"); $$ = create_node("REAL", 0); }
  | CHAR { strcpy(return_type_to_push, "CHAR"); $$ = create_node("CHAR", 0); }
  | BOOL { strcpy(return_type_to_push, "BOOL"); $$ = create_node("BOOL", 0); }
  | INT_PTR { strcpy(return_type_to_push, "INT_PTR"); $$ = create_node("INT_PTR", 0); }
  | REAL_PTR { strcpy(return_type_to_push, "REAL_PTR"); $$ = create_node("REAL_PTR", 0); }
  | CHAR_PTR { strcpy(return_type_to_push, "CHAR_PTR"); $$ = create_node("CHAR_PTR", 0); }
;

type
  : INT { $$ = create_node("INT", 0); }
  | REAL { $$ = create_node("REAL", 0); }
  | CHAR { $$ = create_node("CHAR", 0); }
  | BOOL { $$ = create_node("BOOL", 0); }
  | STRING { $$ = create_node("STRING", 0); }
  | INT_PTR { $$ = create_node("INT_PTR", 0); }
  | REAL_PTR { $$ = create_node("REAL_PTR", 0); }
  | CHAR_PTR { $$ = create_node("CHAR_PTR", 0); }
;

block
  : BEGIN_T inner_block END_T { $$ = $2; }
  | BEGIN_T END_T { $$ = create_node("BLOCK", 0); }
;

body 
  : VAR optional_var_list block { $$ = create_node("VAR_BLOCK", 2, $2, $3); }
  | block { $$ = $1; }
;

inner_block
  : nested_func inner_block {
      if ($2 == NULL)
          $$ = create_node("BLOCK", 1, $1);
      else {
          Node* merged = create_node("BLOCK", $2->child_count + 1);
          merged->children[0] = $1;
          for (int i = 0; i < $2->child_count; i++)
              merged->children[i+1] = $2->children[i];
          $$ = merged;
      }
  }
  | stmts inner_block {
      if ($2 == NULL)
          $$ = create_node("BLOCK", 1, $1);
      else {
          Node* merged = create_node("BLOCK", $2->child_count + 1);
          merged->children[0] = $1;
          for (int i = 0; i < $2->child_count; i++)
              merged->children[i+1] = $2->children[i];
          $$ = merged;
      }
  }
  | nested_func { $$ = create_node("BLOCK", 1, $1); }
  | stmts { $$ = create_node("BLOCK", 1, $1); }
  | /* empty */ { $$ = create_node("BLOCK", 0); }
;

stmts
  : stmt { $$ = $1; }
  | stmt stmts {
      if (strcmp($2->name, "BLOCK") == 0) {
          Node* merged = create_node("BLOCK", $2->child_count + 1);
          merged->children[0] = $1;
          for (int i = 0; i < $2->child_count; i++)
              merged->children[i+1] = $2->children[i];
          $$ = merged;
      } else {
          $$ = create_node("BLOCK", 2, $1, $2);
      }
  }
;

stmt
  : RETURN expr ';' {
      current_func_has_return = 1;
      if (!check_return_type($2)) {
          yyerror("Return type mismatch or returning from procedure");
      }
      $$ = create_node("RET", 1, $2);
  }
  | RETURN ';' {
      current_func_has_return = 1;
      if (strlen(current_return_type) > 0)
          yyerror("Function with return type must return a value");
      $$ = create_node("RET", 0);
  }
  | IDENTIFIER ASSIGN expr ';' {
      $$ = create_node("=", 2, create_node($1, 0), $3);
  }
  | IDENTIFIER ASSIGN CALL IDENTIFIER '(' args ')' ';' {
      Node* call_node = create_node("CALL", 2, create_node($4, 0), $6);
      $$ = create_node("=", 2, create_node($1, 0), call_node);
  }
  | CALL IDENTIFIER '(' args ')' ';' {
      $$ = create_node("CALL", 2, create_node($2, 0), $4);
  }
  | IDENTIFIER '[' expr ']' ASSIGN expr ';' {
      Node* arr_elem = create_node("ARRAY_ELEM", 2, create_node($1, 0), $3);
      $$ = create_node("=", 2, arr_elem, $6);
  }
  | MUL IDENTIFIER ASSIGN expr ';' {
      $$ = create_node("= *", 2, create_node($2, 0), $4);
  }
  | IF expr ':' body ELSE ':' body {
      $$ = create_node("IF-ELSE", 3, $2, $4, $7);
  }
  | IF expr ':' body ELIF expr ':' body ELSE ':' body {
      $$ = create_node("IF-ELIF-ELSE", 5, $2, $4, $6, $8, $11);
  }
  | IF expr ':' body ELIF expr ':' body {
      $$ = create_node("IF-ELIF", 4, $2, $4, $6, $8);
  }
  | IF expr ':' body {
      $$ = create_node("IF", 2, $2, $4);
  }
  | WHILE expr ':' body {
      $$ = create_node("WHILE", 2, $2, $4);
  }
  | DO ':' body WHILE expr ';' {
      $$ = create_node("DO-WHILE", 2, $3, $5);
  }
  | FOR '(' expr ';' expr ';' expr ')' ':' body {
      Node* cond = create_node("FOR_COND", 3, $3, $5, $7);
      $$ = create_node("FOR", 2, cond, $10);
  }
  | VAR optional_var_list block {
      $$ = create_node("BODY", 2, $2, $3);
  }
  | block {
      $$ = $1;
  }
  | VAR optional_var_list stmt {
      $$ = create_node("VAR_BLOCK", 2, $2, $3);
  }
;

/* הצהרות משתנים (var_decls וכו') */
var_decls
  : VAR optional_var_list { $$ = $2; }
  | /* empty */ { $$ = create_node("BLOCK", 0); }
;

optional_var_list
  : var_decl_list { $$ = $1; }
  | /* empty */ { $$ = create_node("BLOCK", 0); }
;

var_decl_list
  : var_decl_list var_single_decl {
      Node* merged = create_node("BLOCK", $1->child_count + 1);
      for (int i = 0; i < $1->child_count; i++)
          merged->children[i] = $1->children[i];
      merged->children[$1->child_count] = $2;
      $$ = merged;
  }
  | var_single_decl { $$ = create_node("BLOCK", 1, $1); }
;

var_single_decl
  : TYPE type ':' IDENTIFIER ';' {
      char temp[100];
      sprintf(temp, "%s %s", $2->name, $4);
      $$ = create_node(temp, 0);
  }
  | TYPE type ':' IDENTIFIER ':' expr ';' {
      char temp[100];
      sprintf(temp, "%s %s", $2->name, $4);
      Node* var_node = create_node(temp, 0);
      $$ = create_node("=", 2, var_node, $6);
  }
  | TYPE BOOL ':' var_init_list ';' {
      $$ = $4;
  }
  | TYPE type ':' string_decl_list ';' {
      $$ = $4;
  }
;

/* אתחול משתנים BOOL */
var_init_list
  : IDENTIFIER ':' expr {
      char temp[100];
      sprintf(temp, "BOOL %s", $1);
      Node* var_node = create_node(temp, 0);
      $$ = create_node("=", 2, var_node, $3);
  }
  | IDENTIFIER ':' expr ',' var_init_list {
      char temp[100];
      sprintf(temp, "BOOL %s", $1);
      Node* var_node = create_node(temp, 0);
      Node* assign_node = create_node("=", 2, var_node, $3);
      Node* merged = create_node("BLOCK", $5->child_count + 1);
      merged->children[0] = assign_node;
      for (int i = 0; i < $5->child_count; i++)
          merged->children[i+1] = $5->children[i];
      $$ = merged;
  }
;

/* אתחול משתנים מסוג מחרוזת */
string_decl_list
  : string_decl_list ',' string_decl {
      Node* merged = create_node("BLOCK", $1->child_count + 1);
      for (int i = 0; i < $1->child_count; i++)
          merged->children[i] = $1->children[i];
      merged->children[$1->child_count] = $3;
      $$ = merged;
  }
  | string_decl {
      $$ = create_node("BLOCK", 1, $1);
  }
;

string_decl
  : IDENTIFIER '[' INT_LITERAL ']' {
      char temp[100];
      sprintf(temp, "STR %s[%s]", $1, $3);
      $$ = create_node(temp, 0);
  }
  | IDENTIFIER '[' INT_LITERAL ']' ':' STRING_LITERAL {
      char temp[100];
      sprintf(temp, "STR %s[%s]:%s", $1, $3, $6);
      $$ = create_node(temp, 0);
  }
;

/* ביטויים מתמטיים ולוגיים */
expr 
  : expr ADD expr { $$ = create_node("+", 2, $1, $3); }
  | expr SUB expr { $$ = create_node("-", 2, $1, $3); }
  | expr MUL expr { $$ = create_node("*", 2, $1, $3); }
  | expr DIV expr { $$ = create_node("/", 2, $1, $3); }
  | expr EQ expr { $$ = create_node("==", 2, $1, $3); }
  | expr NE expr { $$ = create_node("!=", 2, $1, $3); }
  | expr GT expr { $$ = create_node(">", 2, $1, $3); }
  | expr GE expr { $$ = create_node(">=", 2, $1, $3); }
  | expr LT expr { $$ = create_node("<", 2, $1, $3); }
  | expr LE expr { $$ = create_node("<=", 2, $1, $3); }
  | expr AND expr { $$ = create_node("AND", 2, $1, $3); }
  | expr OR expr { $$ = create_node("OR", 2, $1, $3); }
  | NOT expr { $$ = create_node("NOT", 1, $2); }
  | '(' expr ')' { $$ = $2; }
  | SUB expr %prec UMINUS { $$ = create_node("-", 2, create_node("0", 0), $2); }
  | MUL IDENTIFIER { $$ = create_node("*", 1, create_node($2, 0)); }
  | ADDR IDENTIFIER { $$ = create_node("&", 1, create_node($2, 0)); }
  | MUL '(' expr ')' { $$ = create_node("*", 1, $3); }
  | IDENTIFIER { $$ = create_node($1, 0); }
  | IDENTIFIER '(' args ')' { $$ = create_node("CALL", 2, create_node($1, 0), $3); }
  | IDENTIFIER '[' expr ']' { $$ = create_node("ARRAY_ELEM", 2, create_node($1, 0), $3); }
  | INT_LITERAL { $$ = create_node($1, 0); }
  | REAL_LITERAL { $$ = create_node($1, 0); }
  | CHAR_LITERAL { $$ = create_node($1, 0); }
  | STRING_LITERAL { $$ = create_node($1, 0); }
  | TRUE { $$ = create_node("TRUE", 0); }
  | FALSE { $$ = create_node("FALSE", 0); }
  | NULL_T { $$ = create_node("NULL", 0); }
  | PIPE_SYMBOL IDENTIFIER PIPE_SYMBOL { $$ = create_node("LEN", 1, create_node($2, 0)); }
;

/* קריאות לפונקציה: ארגומנטים */
args 
  : expr { $$ = create_node("ARGS", 1, $1); }
  | expr ',' args {
      if (strcmp($3->name, "ARGS") == 0) {
          Node* merged = create_node("ARGS", $3->child_count + 1);
          merged->children[0] = $1;
          for (int i = 0; i < $3->child_count; i++)
              merged->children[i+1] = $3->children[i];
          $$ = merged;
      } else {
          $$ = create_node("ARGS", 2, $1, $3);
      }
  }
  | /* empty */ { $$ = create_node("ARGS NONE", 0); }
;

%%

/* הדפסת שגיאות יפות */
void yyerror(const char* s) {
    if (param_error) {
        printf("Syntax error at line %d: no type defined\n", yylineno);
        param_error = 0;
    }
    else if (comma_error) {
        printf("Syntax error at line %d: parameters must be separated by semicolon\n", yylineno);
        comma_error = 0;
    }
    else if (return_type_error) {
        printf("Semantic error at line %d: Return type mismatch\n", yylineno);
        return_type_error = 0;
    }
    else if (missing_return_error) {
        printf("Semantic error at line %d: Function with return type must end with a return statement\n", yylineno);
        missing_return_error = 0;
    }
    else if (proc_return_error) {
        printf("Semantic error at line %d: Cannot return a value from a procedure\n", yylineno);
        proc_return_error = 0;
    }
    else {
        printf("Syntax error at line %d: %s\n", yylineno, s);
    }
    exit(1);
}

/* MAIN שמריץ את ה-parser */
int main() {
    if (yyparse() == 0) {
        if (!has_main) {
            fprintf(stderr, "Error: Program must have exactly one _main_() procedure\n");
            return 1;
        }
        print_ast(root, 0);
    }
    return 0;
}

