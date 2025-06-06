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
    DEF = 258,                     /* DEF  */
    RETURNS = 259,                 /* RETURNS  */
    RETURN = 260,                  /* RETURN  */
    IF = 261,                      /* IF  */
    ELSE = 262,                    /* ELSE  */
    ELIF = 263,                    /* ELIF  */
    WHILE = 264,                   /* WHILE  */
    FOR = 265,                     /* FOR  */
    DO = 266,                      /* DO  */
    VAR = 267,                     /* VAR  */
    BEGIN_T = 268,                 /* BEGIN_T  */
    END_T = 269,                   /* END_T  */
    CALL = 270,                    /* CALL  */
    NULL_T = 271,                  /* NULL_T  */
    TRUE = 272,                    /* TRUE  */
    FALSE = 273,                   /* FALSE  */
    TYPE = 274,                    /* TYPE  */
    INT = 275,                     /* INT  */
    REAL = 276,                    /* REAL  */
    CHAR = 277,                    /* CHAR  */
    BOOL = 278,                    /* BOOL  */
    STRING = 279,                  /* STRING  */
    INT_PTR = 280,                 /* INT_PTR  */
    REAL_PTR = 281,                /* REAL_PTR  */
    CHAR_PTR = 282,                /* CHAR_PTR  */
    AND = 283,                     /* AND  */
    OR = 284,                      /* OR  */
    NOT = 285,                     /* NOT  */
    EQ = 286,                      /* EQ  */
    NE = 287,                      /* NE  */
    LE = 288,                      /* LE  */
    GE = 289,                      /* GE  */
    LT = 290,                      /* LT  */
    GT = 291,                      /* GT  */
    ASSIGN = 292,                  /* ASSIGN  */
    ADD = 293,                     /* ADD  */
    SUB = 294,                     /* SUB  */
    MUL = 295,                     /* MUL  */
    DIV = 296,                     /* DIV  */
    ADDR = 297,                    /* ADDR  */
    DEREF = 298,                   /* DEREF  */
    PIPE_SYMBOL = 299,             /* PIPE_SYMBOL  */
    IDENTIFIER = 300,              /* IDENTIFIER  */
    INT_LITERAL = 301,             /* INT_LITERAL  */
    REAL_LITERAL = 302,            /* REAL_LITERAL  */
    CHAR_LITERAL = 303,            /* CHAR_LITERAL  */
    STRING_LITERAL = 304,          /* STRING_LITERAL  */
    BITWISE_OR = 305,              /* BITWISE_OR  */
    BITWISE_AND = 306,             /* BITWISE_AND  */
    BITWISE_NOT = 307,             /* BITWISE_NOT  */
    UMINUS = 308,                  /* UMINUS  */
    LOWER_THAN_ELSE = 309          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 708 "parser.y"

    char* str;
    Node* node;

#line 123 "parser.tab.h"

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
