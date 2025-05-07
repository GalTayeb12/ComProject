/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    TYPE = 259,                    /* TYPE  */
    STRING_LITERAL = 260,          /* STRING_LITERAL  */
    CHAR_LITERAL = 261,            /* CHAR_LITERAL  */
    BOOL_LITERAL = 262,            /* BOOL_LITERAL  */
    INT_LITERAL = 263,             /* INT_LITERAL  */
    REAL_LITERAL = 264,            /* REAL_LITERAL  */
    DEF = 265,                     /* DEF  */
    RETURNS = 266,                 /* RETURNS  */
    RETURN = 267,                  /* RETURN  */
    IF = 268,                      /* IF  */
    ELSE = 269,                    /* ELSE  */
    ELIF = 270,                    /* ELIF  */
    WHILE = 271,                   /* WHILE  */
    FOR = 272,                     /* FOR  */
    DO = 273,                      /* DO  */
    VAR = 274,                     /* VAR  */
    BEGIN_T = 275,                 /* BEGIN_T  */
    END_T = 276,                   /* END_T  */
    CALL = 277,                    /* CALL  */
    NULL_T = 278,                  /* NULL_T  */
    TRUE = 279,                    /* TRUE  */
    FALSE = 280,                   /* FALSE  */
    VOID = 281,                    /* VOID  */
    INT = 282,                     /* INT  */
    REAL = 283,                    /* REAL  */
    CHAR = 284,                    /* CHAR  */
    BOOL = 285,                    /* BOOL  */
    STRING = 286,                  /* STRING  */
    INT_PTR = 287,                 /* INT_PTR  */
    REAL_PTR = 288,                /* REAL_PTR  */
    CHAR_PTR = 289,                /* CHAR_PTR  */
    ADD = 290,                     /* ADD  */
    SUB = 291,                     /* SUB  */
    MUL = 292,                     /* MUL  */
    DIV = 293,                     /* DIV  */
    ASSIGN = 294,                  /* ASSIGN  */
    EQ = 295,                      /* EQ  */
    NEQ = 296,                     /* NEQ  */
    LEQ = 297,                     /* LEQ  */
    GEQ = 298,                     /* GEQ  */
    LT = 299,                      /* LT  */
    GT = 300,                      /* GT  */
    AND = 301,                     /* AND  */
    OR = 302,                      /* OR  */
    NOT = 303,                     /* NOT  */
    ADDR = 304,                    /* ADDR  */
    PIPE_SYMBOL = 305,             /* PIPE_SYMBOL  */
    UMINUS = 306                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 25 "parser.y"

    Node* node;
    char* str;

#line 120 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
