%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "parser.tab.h"

extern YYSTYPE yylval;
%}

%option noyywrap
%option yylineno

DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          ({LETTER}|_)({LETTER}|{DIGIT}|_)* 

%%

"def"           { return DEF; }
"returns"       { return RETURNS; }
"return"        { return RETURN; }
"if"            { return IF; }
"else"          { return ELSE; }
"elif"          { return ELIF; }
"while"         { return WHILE; }
"for"           { return FOR; }
"do"            { return DO; }
"var"           { return VAR; }
"type"          { return TYPE; }
"begin"         { return BEGIN_T; }
"end"           { return END_T; }
"call"          { return CALL; }
"null"          { return NULL_T; }
"TRUE"          { return TRUE; }
"FALSE"         { return FALSE; }

"int"           { return INT; }
"real"          { return REAL; }
"char"          { return CHAR; }
"bool"          { return BOOL; }
"string"        { return STRING; }
"int*"          { return INT_PTR; }
"real*"         { return REAL_PTR; }
"char*"         { return CHAR_PTR; }

"and"           { return AND; }
"or"            { return OR; }
"not"           { return NOT; }

"=="            { return EQ; }
"!="            { return NE; }
"<="            { return LE; }
">="            { return GE; }
"<"             { return LT; }
">"             { return GT; }
"="             { return ASSIGN; }
"+"             { return ADD; }
"-"             { return SUB; }
"*"             { return MUL; }
"/"             { return DIV; }
"&"             { return ADDR; }

";"             { return ';'; }
","             { return ','; }
":"             { return ':'; }
"("             { return '('; }
")"             { return ')'; }
"["             { return '['; }
"]"             { return ']'; }
"|"             { return PIPE_SYMBOL; }

{ID}            { yylval.str = strdup(yytext); return IDENTIFIER; }
{DIGIT}+        { yylval.str = strdup(yytext); return INT_LITERAL; }
{DIGIT}+"."{DIGIT}+([Ee][+-]?{DIGIT}+)?  { yylval.str = strdup(yytext); return REAL_LITERAL; }
\'([^\\\n\'])\'  { yylval.str = strdup(yytext); return CHAR_LITERAL; }
\"([^\"\n]*)\"   { yylval.str = strdup(yytext); return STRING_LITERAL; }

[ \t\r\n]+      { /* ignore whitespace */ }

"#->"([^<]|"<"[^-]|"<-"[^#])*"<-#"  { /* ignore comment */ }

.               { printf("Unknown character: %s\n", yytext); exit(1); }

%%
