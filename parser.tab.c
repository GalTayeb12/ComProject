/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

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

#line 206 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_DEF = 3,                        /* DEF  */
  YYSYMBOL_RETURNS = 4,                    /* RETURNS  */
  YYSYMBOL_RETURN = 5,                     /* RETURN  */
  YYSYMBOL_IF = 6,                         /* IF  */
  YYSYMBOL_ELSE = 7,                       /* ELSE  */
  YYSYMBOL_ELIF = 8,                       /* ELIF  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_FOR = 10,                       /* FOR  */
  YYSYMBOL_DO = 11,                        /* DO  */
  YYSYMBOL_VAR = 12,                       /* VAR  */
  YYSYMBOL_BEGIN_T = 13,                   /* BEGIN_T  */
  YYSYMBOL_END_T = 14,                     /* END_T  */
  YYSYMBOL_CALL = 15,                      /* CALL  */
  YYSYMBOL_NULL_T = 16,                    /* NULL_T  */
  YYSYMBOL_TRUE = 17,                      /* TRUE  */
  YYSYMBOL_FALSE = 18,                     /* FALSE  */
  YYSYMBOL_TYPE = 19,                      /* TYPE  */
  YYSYMBOL_INT = 20,                       /* INT  */
  YYSYMBOL_REAL = 21,                      /* REAL  */
  YYSYMBOL_CHAR = 22,                      /* CHAR  */
  YYSYMBOL_BOOL = 23,                      /* BOOL  */
  YYSYMBOL_STRING = 24,                    /* STRING  */
  YYSYMBOL_INT_PTR = 25,                   /* INT_PTR  */
  YYSYMBOL_REAL_PTR = 26,                  /* REAL_PTR  */
  YYSYMBOL_CHAR_PTR = 27,                  /* CHAR_PTR  */
  YYSYMBOL_AND = 28,                       /* AND  */
  YYSYMBOL_OR = 29,                        /* OR  */
  YYSYMBOL_NOT = 30,                       /* NOT  */
  YYSYMBOL_EQ = 31,                        /* EQ  */
  YYSYMBOL_NE = 32,                        /* NE  */
  YYSYMBOL_LE = 33,                        /* LE  */
  YYSYMBOL_GE = 34,                        /* GE  */
  YYSYMBOL_LT = 35,                        /* LT  */
  YYSYMBOL_GT = 36,                        /* GT  */
  YYSYMBOL_ASSIGN = 37,                    /* ASSIGN  */
  YYSYMBOL_ADD = 38,                       /* ADD  */
  YYSYMBOL_SUB = 39,                       /* SUB  */
  YYSYMBOL_MUL = 40,                       /* MUL  */
  YYSYMBOL_DIV = 41,                       /* DIV  */
  YYSYMBOL_ADDR = 42,                      /* ADDR  */
  YYSYMBOL_DEREF = 43,                     /* DEREF  */
  YYSYMBOL_PIPE_SYMBOL = 44,               /* PIPE_SYMBOL  */
  YYSYMBOL_IDENTIFIER = 45,                /* IDENTIFIER  */
  YYSYMBOL_INT_LITERAL = 46,               /* INT_LITERAL  */
  YYSYMBOL_REAL_LITERAL = 47,              /* REAL_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 48,              /* CHAR_LITERAL  */
  YYSYMBOL_STRING_LITERAL = 49,            /* STRING_LITERAL  */
  YYSYMBOL_UMINUS = 50,                    /* UMINUS  */
  YYSYMBOL_LOWER_THAN_ELSE = 51,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_52_ = 52,                       /* '('  */
  YYSYMBOL_53_ = 53,                       /* ')'  */
  YYSYMBOL_54_ = 54,                       /* ':'  */
  YYSYMBOL_55_ = 55,                       /* ';'  */
  YYSYMBOL_56_ = 56,                       /* '['  */
  YYSYMBOL_57_ = 57,                       /* ']'  */
  YYSYMBOL_58_ = 58,                       /* ','  */
  YYSYMBOL_YYACCEPT = 59,                  /* $accept  */
  YYSYMBOL_program = 60,                   /* program  */
  YYSYMBOL_funcs = 61,                     /* funcs  */
  YYSYMBOL_func = 62,                      /* func  */
  YYSYMBOL_nested_func = 63,               /* nested_func  */
  YYSYMBOL_func_header = 64,               /* func_header  */
  YYSYMBOL_parameters = 65,                /* parameters  */
  YYSYMBOL_parameter = 66,                 /* parameter  */
  YYSYMBOL_ret_type = 67,                  /* ret_type  */
  YYSYMBOL_type = 68,                      /* type  */
  YYSYMBOL_block = 69,                     /* block  */
  YYSYMBOL_body = 70,                      /* body  */
  YYSYMBOL_inner_block = 71,               /* inner_block  */
  YYSYMBOL_stmts = 72,                     /* stmts  */
  YYSYMBOL_stmt = 73,                      /* stmt  */
  YYSYMBOL_var_decls = 74,                 /* var_decls  */
  YYSYMBOL_optional_var_list = 75,         /* optional_var_list  */
  YYSYMBOL_var_decl_list = 76,             /* var_decl_list  */
  YYSYMBOL_var_single_decl = 77,           /* var_single_decl  */
  YYSYMBOL_var_init_list = 78,             /* var_init_list  */
  YYSYMBOL_string_decl_list = 79,          /* string_decl_list  */
  YYSYMBOL_string_decl = 80,               /* string_decl  */
  YYSYMBOL_expr = 81,                      /* expr  */
  YYSYMBOL_args = 82                       /* args  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   560

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  105
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  219

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   306


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      52,    53,     2,     2,    58,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    54,    55,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    56,     2,    57,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   169,   169,   173,   174,   175,   179,   190,   201,   210,
     222,   229,   232,   238,   243,   251,   252,   253,   254,   255,
     256,   257,   261,   262,   263,   264,   265,   266,   267,   268,
     272,   273,   277,   278,   282,   293,   304,   305,   306,   310,
     311,   325,   332,   338,   341,   345,   348,   352,   355,   358,
     361,   364,   367,   370,   373,   377,   380,   383,   390,   391,
     395,   396,   400,   407,   411,   416,   422,   425,   432,   438,
     453,   460,   466,   471,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   513,   514,   525
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "DEF", "RETURNS",
  "RETURN", "IF", "ELSE", "ELIF", "WHILE", "FOR", "DO", "VAR", "BEGIN_T",
  "END_T", "CALL", "NULL_T", "TRUE", "FALSE", "TYPE", "INT", "REAL",
  "CHAR", "BOOL", "STRING", "INT_PTR", "REAL_PTR", "CHAR_PTR", "AND", "OR",
  "NOT", "EQ", "NE", "LE", "GE", "LT", "GT", "ASSIGN", "ADD", "SUB", "MUL",
  "DIV", "ADDR", "DEREF", "PIPE_SYMBOL", "IDENTIFIER", "INT_LITERAL",
  "REAL_LITERAL", "CHAR_LITERAL", "STRING_LITERAL", "UMINUS",
  "LOWER_THAN_ELSE", "'('", "')'", "':'", "';'", "'['", "']'", "','",
  "$accept", "program", "funcs", "func", "nested_func", "func_header",
  "parameters", "parameter", "ret_type", "type", "block", "body",
  "inner_block", "stmts", "stmt", "var_decls", "optional_var_list",
  "var_decl_list", "var_single_decl", "var_init_list", "string_decl_list",
  "string_decl", "expr", "args", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-113)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      26,    21,    42,  -113,    26,    32,    46,  -113,  -113,    80,
      88,    61,   172,  -113,    80,  -113,   102,  -113,    36,    56,
      55,  -113,  -113,  -113,    68,  -113,  -113,  -113,  -113,    78,
    -113,   132,   184,   184,    81,    87,    80,  -113,    94,    98,
     -32,   125,    32,  -113,   130,   125,     3,  -113,   100,    92,
      97,    61,   107,   109,  -113,  -113,  -113,   184,   184,   -35,
     110,   111,   -45,  -113,  -113,  -113,  -113,   184,  -113,   268,
     412,   429,   184,    74,     3,   105,   121,   173,   184,  -113,
      88,  -113,  -113,  -113,  -113,   114,   157,  -113,   112,   108,
      -3,    12,  -113,  -113,  -113,  -113,   184,  -113,   120,   184,
     184,   463,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,  -113,    74,    74,   286,    80,  -113,
     160,  -113,  -113,   184,   184,   128,   304,   234,  -113,  -113,
     533,   184,  -113,   184,  -113,   129,  -113,   137,   479,  -113,
     206,   133,   248,  -113,    -1,   511,    85,    85,   -18,   -18,
     -18,   -18,    51,    51,  -113,  -113,    89,  -113,   184,    88,
     184,   151,   322,   131,  -113,   148,  -113,  -113,  -113,  -113,
    -113,  -113,  -113,  -113,   220,   340,   149,   152,  -113,  -113,
     184,  -113,  -113,   153,   184,   358,  -113,   376,   150,  -113,
     184,   184,   107,  -113,   155,  -113,    74,   446,   184,  -113,
    -113,   158,   394,  -113,   161,  -113,    74,   495,   188,  -113,
    -113,   209,   196,  -113,   203,    74,    74,  -113,  -113
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,     0,     2,     3,    59,     0,     1,     4,    61,
       0,    12,     0,    58,    60,    63,     0,     6,     0,     0,
      11,    22,    23,    24,     0,    26,    27,    28,    29,     0,
      62,     0,     0,     0,     0,     0,    61,    31,     0,     0,
       0,    36,    59,    56,     0,    37,    39,    25,     0,     0,
       0,    12,     0,     0,   101,    99,   100,     0,     0,     0,
       0,     0,    92,    95,    96,    97,    98,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    34,
       0,    30,    35,    40,    14,     0,     9,    10,     0,     0,
       0,     0,    71,    86,    88,    89,     0,    90,     0,   105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    41,     0,     0,     0,    61,    33,
       0,    55,    57,   105,     0,     0,     0,     0,     7,    13,
       0,     0,    66,     0,    64,     0,    67,     0,     0,   102,
     103,     0,     0,    87,    84,    85,    78,    79,    83,    81,
      82,    80,    74,    75,    76,    77,    51,    52,     0,     0,
       0,     0,     0,     0,    43,     0,    15,    16,    17,    18,
      19,    20,    21,     8,    68,     0,     0,     0,    70,    91,
     105,    93,    94,     0,     0,     0,    32,     0,     0,    47,
     105,     0,     0,    65,    72,   104,     0,     0,     0,    53,
      45,     0,     0,    69,     0,    48,     0,     0,     0,    46,
      73,    50,     0,    44,     0,     0,     0,    54,    49
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -113,  -113,   223,  -113,  -113,    45,   239,  -113,  -113,   253,
       9,  -112,    23,   246,   211,   251,   -30,  -113,   280,   103,
    -113,   174,   -31,   -95
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     4,    41,    42,    19,    20,   173,    29,
     119,   120,    44,    45,    46,    10,    13,    14,    15,    89,
      91,    92,   140,   141
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      69,    70,    71,   156,   157,    77,    74,    99,    31,    32,
      95,   100,    33,    34,    35,    36,    16,    96,    38,    17,
     110,   111,   112,   113,    78,    43,    93,    94,   161,     1,
     104,   105,   106,   107,   108,   109,   101,   110,   111,   112,
     113,   117,     7,    39,     9,     5,   126,   127,    40,     5,
      43,   133,   134,   135,    43,    43,    21,    22,    23,    47,
      25,    26,    27,    28,    79,   138,     6,   136,    82,   142,
     137,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   121,   205,   195,   118,    16,   159,   128,
      48,   112,   113,   162,   211,   201,   183,   184,    11,    12,
     174,    16,   175,   217,   218,     1,    18,    31,    32,    50,
      51,    33,    34,    35,    36,    16,    37,    38,   106,   107,
     108,   109,    52,   110,   111,   112,   113,   185,     1,   187,
      31,    32,    53,    72,    33,    34,    35,    36,    16,    75,
      38,    73,    39,    76,    81,    84,    85,    40,    54,    55,
      56,    86,    88,   197,    90,    97,    98,   123,   124,   129,
     202,   130,    57,   132,   139,    39,   131,   207,   186,   160,
      40,    58,    59,   163,    60,   176,    61,    62,    63,    64,
      65,    66,   177,   190,    67,   191,   181,    68,   125,    54,
      55,    56,    21,    22,    23,    24,    25,    26,    27,    28,
      54,    55,    56,    57,   188,   200,   194,   196,   135,   204,
     210,   208,    58,    59,    57,    60,   214,    61,    62,    63,
      64,    65,    66,    58,    59,    67,    60,     8,    61,    62,
      63,    64,    65,    66,   102,   103,    67,   104,   105,   106,
     107,   108,   109,   213,   110,   111,   112,   113,   102,   103,
     215,   104,   105,   106,   107,   108,   109,   216,   110,   111,
     112,   113,   102,   103,   180,   104,   105,   106,   107,   108,
     109,    49,   110,   111,   112,   113,   102,   103,   192,   104,
     105,   106,   107,   108,   109,   122,   110,   111,   112,   113,
      87,   165,    83,    80,    30,   203,   102,   103,     0,   104,
     105,   106,   107,   108,   109,   182,   110,   111,   112,   113,
       0,   178,     0,     0,   102,   103,     0,   104,   105,   106,
     107,   108,   109,   114,   110,   111,   112,   113,     0,     0,
       0,     0,   102,   103,     0,   104,   105,   106,   107,   108,
     109,   158,   110,   111,   112,   113,     0,     0,     0,     0,
     102,   103,     0,   104,   105,   106,   107,   108,   109,   164,
     110,   111,   112,   113,     0,     0,     0,     0,   102,   103,
       0,   104,   105,   106,   107,   108,   109,   189,   110,   111,
     112,   113,     0,     0,     0,     0,   102,   103,     0,   104,
     105,   106,   107,   108,   109,   193,   110,   111,   112,   113,
       0,     0,     0,     0,   102,   103,     0,   104,   105,   106,
     107,   108,   109,   198,   110,   111,   112,   113,     0,     0,
       0,     0,   102,   103,     0,   104,   105,   106,   107,   108,
     109,   199,   110,   111,   112,   113,     0,     0,     0,     0,
     102,   103,     0,   104,   105,   106,   107,   108,   109,   209,
     110,   111,   112,   113,     0,     0,     0,   102,   103,     0,
     104,   105,   106,   107,   108,   109,   115,   110,   111,   112,
     113,     0,     0,     0,   102,   103,     0,   104,   105,   106,
     107,   108,   109,   116,   110,   111,   112,   113,     0,     0,
       0,   102,   103,     0,   104,   105,   106,   107,   108,   109,
     206,   110,   111,   112,   113,     0,     0,   102,   103,     0,
     104,   105,   106,   107,   108,   109,   143,   110,   111,   112,
     113,     0,     0,   102,   103,     0,   104,   105,   106,   107,
     108,   109,   179,   110,   111,   112,   113,     0,     0,   102,
       0,     0,   104,   105,   106,   107,   108,   109,   212,   110,
     111,   112,   113,   166,   167,   168,   169,     0,   170,   171,
     172
};

static const yytype_int16 yycheck[] =
{
      31,    32,    33,   115,   116,    37,    36,    52,     5,     6,
      45,    56,     9,    10,    11,    12,    13,    52,    15,    10,
      38,    39,    40,    41,    56,    16,    57,    58,   123,     3,
      31,    32,    33,    34,    35,    36,    67,    38,    39,    40,
      41,    72,     0,    40,    12,     0,    77,    78,    45,     4,
      41,    54,    55,    56,    45,    46,    20,    21,    22,    23,
      24,    25,    26,    27,    41,    96,    45,    55,    45,   100,
      58,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,    74,   196,   180,    12,    13,   118,    80,
      54,    40,    41,   124,   206,   190,     7,     8,    52,    19,
     131,    13,   133,   215,   216,     3,    45,     5,     6,    53,
      55,     9,    10,    11,    12,    13,    14,    15,    33,    34,
      35,    36,    54,    38,    39,    40,    41,   158,     3,   160,
       5,     6,    54,    52,     9,    10,    11,    12,    13,    45,
      15,    54,    40,    45,    14,    45,    54,    45,    16,    17,
      18,    54,    45,   184,    45,    45,    45,    52,    37,    45,
     191,     4,    30,    55,    44,    40,    54,   198,   159,     9,
      45,    39,    40,    45,    42,    46,    44,    45,    46,    47,
      48,    49,    45,    52,    52,    37,    53,    55,    15,    16,
      17,    18,    20,    21,    22,    23,    24,    25,    26,    27,
      16,    17,    18,    30,    53,    55,    57,    54,    56,    54,
      49,    53,    39,    40,    30,    42,     7,    44,    45,    46,
      47,    48,    49,    39,    40,    52,    42,     4,    44,    45,
      46,    47,    48,    49,    28,    29,    52,    31,    32,    33,
      34,    35,    36,    55,    38,    39,    40,    41,    28,    29,
      54,    31,    32,    33,    34,    35,    36,    54,    38,    39,
      40,    41,    28,    29,    58,    31,    32,    33,    34,    35,
      36,    18,    38,    39,    40,    41,    28,    29,    58,    31,
      32,    33,    34,    35,    36,    74,    38,    39,    40,    41,
      51,    57,    46,    42,    14,   192,    28,    29,    -1,    31,
      32,    33,    34,    35,    36,    57,    38,    39,    40,    41,
      -1,   137,    -1,    -1,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    55,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    28,    29,    -1,    31,    32,    33,    34,    35,
      36,    55,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      28,    29,    -1,    31,    32,    33,    34,    35,    36,    55,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    28,    29,
      -1,    31,    32,    33,    34,    35,    36,    55,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    28,    29,    -1,    31,
      32,    33,    34,    35,    36,    55,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    55,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    28,    29,    -1,    31,    32,    33,    34,    35,
      36,    55,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      28,    29,    -1,    31,    32,    33,    34,    35,    36,    55,
      38,    39,    40,    41,    -1,    -1,    -1,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    54,    38,    39,    40,
      41,    -1,    -1,    -1,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    54,    38,    39,    40,    41,    -1,    -1,
      -1,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      54,    38,    39,    40,    41,    -1,    -1,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    53,    38,    39,    40,
      41,    -1,    -1,    28,    29,    -1,    31,    32,    33,    34,
      35,    36,    53,    38,    39,    40,    41,    -1,    -1,    28,
      -1,    -1,    31,    32,    33,    34,    35,    36,    53,    38,
      39,    40,    41,    20,    21,    22,    23,    -1,    25,    26,
      27
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    60,    61,    62,    64,    45,     0,    61,    12,
      74,    52,    19,    75,    76,    77,    13,    69,    45,    65,
      66,    20,    21,    22,    23,    24,    25,    26,    27,    68,
      77,     5,     6,     9,    10,    11,    12,    14,    15,    40,
      45,    63,    64,    69,    71,    72,    73,    23,    54,    68,
      53,    55,    54,    54,    16,    17,    18,    30,    39,    40,
      42,    44,    45,    46,    47,    48,    49,    52,    55,    81,
      81,    81,    52,    54,    75,    45,    45,    37,    56,    71,
      74,    14,    71,    72,    45,    54,    54,    65,    45,    78,
      45,    79,    80,    81,    81,    45,    52,    45,    45,    52,
      56,    81,    28,    29,    31,    32,    33,    34,    35,    36,
      38,    39,    40,    41,    55,    54,    54,    81,    12,    69,
      70,    69,    73,    52,    37,    15,    81,    81,    69,    45,
       4,    54,    55,    54,    55,    56,    55,    58,    81,    44,
      81,    82,    81,    53,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    70,    70,    55,    75,
       9,    82,    81,    45,    55,    57,    20,    21,    22,    23,
      25,    26,    27,    67,    81,    81,    46,    45,    80,    53,
      58,    53,    57,     7,     8,    81,    69,    81,    53,    55,
      52,    37,    58,    55,    57,    82,    54,    81,    55,    55,
      55,    82,    81,    78,    54,    70,    54,    81,    53,    55,
      49,    70,    53,    55,     7,    54,    54,    70,    70
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    59,    60,    61,    61,    61,    62,    63,    64,    64,
      65,    65,    65,    66,    66,    67,    67,    67,    67,    67,
      67,    67,    68,    68,    68,    68,    68,    68,    68,    68,
      69,    69,    70,    70,    71,    71,    71,    71,    71,    72,
      72,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    74,    74,
      75,    75,    76,    76,    77,    77,    77,    77,    78,    78,
      79,    79,    80,    80,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    82,    82,    82
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     0,     3,     3,     8,     6,
       3,     1,     0,     4,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     2,     3,     1,     2,     2,     1,     1,     0,     1,
       2,     3,     2,     4,     8,     6,     7,     5,     7,    11,
       8,     4,     4,     6,    10,     3,     1,     3,     2,     0,
       1,     0,     2,     1,     5,     7,     5,     5,     3,     5,
       3,     1,     4,     6,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     2,     2,
       2,     4,     1,     4,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: funcs  */
#line 169 "parser.y"
          { root = create_node("CODE", 1, (yyvsp[0].node)); }
#line 1486 "parser.tab.c"
    break;

  case 3: /* funcs: func  */
#line 173 "parser.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1492 "parser.tab.c"
    break;

  case 4: /* funcs: func funcs  */
#line 174 "parser.y"
               { (yyval.node) = create_node("FUNC", 2, (yyvsp[-1].node), (yyvsp[0].node)); }
#line 1498 "parser.tab.c"
    break;

  case 5: /* funcs: %empty  */
#line 175 "parser.y"
                { (yyval.node) = NULL; }
#line 1504 "parser.tab.c"
    break;

  case 6: /* func: func_header var_decls block  */
#line 179 "parser.y"
                                {
      if (!ends_with_return((yyvsp[0].node)) && strlen(current_return_type) > 0) {
          missing_return_error = 1;
          yyerror("Function with return type must end with a return statement");
      }
      (yyval.node) = create_node((yyvsp[-2].node)->name, 3, (yyvsp[-2].node)->children[0], (yyvsp[-2].node)->children[1], create_node("BODY", 2, (yyvsp[-1].node), (yyvsp[0].node)));
      pop_return_type();
  }
#line 1517 "parser.tab.c"
    break;

  case 7: /* nested_func: func_header var_decls block  */
#line 190 "parser.y"
                                {
      if (!ends_with_return((yyvsp[0].node)) && strlen(current_return_type) > 0) {
          missing_return_error = 1;
          yyerror("Function with return type must end with a return statement");
      }
      (yyval.node) = create_node((yyvsp[-2].node)->name, 3, (yyvsp[-2].node)->children[0], (yyvsp[-2].node)->children[1], create_node("BODY", 2, (yyvsp[-1].node), (yyvsp[0].node)));
      pop_return_type();
  }
#line 1530 "parser.tab.c"
    break;

  case 8: /* func_header: DEF IDENTIFIER '(' parameters ')' ':' RETURNS ret_type  */
#line 201 "parser.y"
                                                           {
      push_return_type(return_type_to_push);
      Node* params = (yyvsp[-4].node);
      Node* ret_type_node = (yyvsp[0].node);
      if (strcmp((yyvsp[-6].str), "_main_") == 0)
          yyerror("Error: _main_() cannot return a value");
      add_function((yyvsp[-6].str));
      (yyval.node) = create_node((yyvsp[-6].str), 2, params, ret_type_node);
  }
#line 1544 "parser.tab.c"
    break;

  case 9: /* func_header: DEF IDENTIFIER '(' parameters ')' ':'  */
#line 210 "parser.y"
                                          {
      push_return_type(""); 
      Node* params = (yyvsp[-2].node);
      Node* ret_type_node = create_node("RET NONE", 0);
      if (strcmp((yyvsp[-4].str), "_main_") == 0)
          has_main = 1;
      add_function((yyvsp[-4].str));
      (yyval.node) = create_node((yyvsp[-4].str), 2, params, ret_type_node);
  }
#line 1558 "parser.tab.c"
    break;

  case 10: /* parameters: parameter ';' parameters  */
#line 222 "parser.y"
                             {
      Node* pars = create_node("PARS", (yyvsp[0].node)->child_count + 1);
      pars->children[0] = (yyvsp[-2].node);
      for (int i = 0; i < (yyvsp[0].node)->child_count; i++)
          pars->children[i+1] = (yyvsp[0].node)->children[i];
      (yyval.node) = pars;
  }
#line 1570 "parser.tab.c"
    break;

  case 11: /* parameters: parameter  */
#line 229 "parser.y"
              {
      (yyval.node) = create_node("PARS", 1, (yyvsp[0].node));
  }
#line 1578 "parser.tab.c"
    break;

  case 12: /* parameters: %empty  */
#line 232 "parser.y"
                {
      (yyval.node) = create_node("PARS NONE", 0);
  }
#line 1586 "parser.tab.c"
    break;

  case 13: /* parameter: IDENTIFIER type ':' IDENTIFIER  */
#line 238 "parser.y"
                                   {
      char temp[100];
      sprintf(temp, "par%s %s %s", (yyvsp[-3].str) + 3, (yyvsp[-2].node)->name, (yyvsp[0].str));
      (yyval.node) = create_node(temp, 0);
  }
#line 1596 "parser.tab.c"
    break;

  case 14: /* parameter: IDENTIFIER ':' IDENTIFIER  */
#line 243 "parser.y"
                              {
      param_error = 1;
      yyerror("no type defined");
      (yyval.node) = create_node("ERROR", 0);
  }
#line 1606 "parser.tab.c"
    break;

  case 15: /* ret_type: INT  */
#line 251 "parser.y"
        { strcpy(return_type_to_push, "INT"); (yyval.node) = create_node("INT", 0); }
#line 1612 "parser.tab.c"
    break;

  case 16: /* ret_type: REAL  */
#line 252 "parser.y"
         { strcpy(return_type_to_push, "REAL"); (yyval.node) = create_node("REAL", 0); }
#line 1618 "parser.tab.c"
    break;

  case 17: /* ret_type: CHAR  */
#line 253 "parser.y"
         { strcpy(return_type_to_push, "CHAR"); (yyval.node) = create_node("CHAR", 0); }
#line 1624 "parser.tab.c"
    break;

  case 18: /* ret_type: BOOL  */
#line 254 "parser.y"
         { strcpy(return_type_to_push, "BOOL"); (yyval.node) = create_node("BOOL", 0); }
#line 1630 "parser.tab.c"
    break;

  case 19: /* ret_type: INT_PTR  */
#line 255 "parser.y"
            { strcpy(return_type_to_push, "INT_PTR"); (yyval.node) = create_node("INT_PTR", 0); }
#line 1636 "parser.tab.c"
    break;

  case 20: /* ret_type: REAL_PTR  */
#line 256 "parser.y"
             { strcpy(return_type_to_push, "REAL_PTR"); (yyval.node) = create_node("REAL_PTR", 0); }
#line 1642 "parser.tab.c"
    break;

  case 21: /* ret_type: CHAR_PTR  */
#line 257 "parser.y"
             { strcpy(return_type_to_push, "CHAR_PTR"); (yyval.node) = create_node("CHAR_PTR", 0); }
#line 1648 "parser.tab.c"
    break;

  case 22: /* type: INT  */
#line 261 "parser.y"
        { (yyval.node) = create_node("INT", 0); }
#line 1654 "parser.tab.c"
    break;

  case 23: /* type: REAL  */
#line 262 "parser.y"
         { (yyval.node) = create_node("REAL", 0); }
#line 1660 "parser.tab.c"
    break;

  case 24: /* type: CHAR  */
#line 263 "parser.y"
         { (yyval.node) = create_node("CHAR", 0); }
#line 1666 "parser.tab.c"
    break;

  case 25: /* type: BOOL  */
#line 264 "parser.y"
         { (yyval.node) = create_node("BOOL", 0); }
#line 1672 "parser.tab.c"
    break;

  case 26: /* type: STRING  */
#line 265 "parser.y"
           { (yyval.node) = create_node("STRING", 0); }
#line 1678 "parser.tab.c"
    break;

  case 27: /* type: INT_PTR  */
#line 266 "parser.y"
            { (yyval.node) = create_node("INT_PTR", 0); }
#line 1684 "parser.tab.c"
    break;

  case 28: /* type: REAL_PTR  */
#line 267 "parser.y"
             { (yyval.node) = create_node("REAL_PTR", 0); }
#line 1690 "parser.tab.c"
    break;

  case 29: /* type: CHAR_PTR  */
#line 268 "parser.y"
             { (yyval.node) = create_node("CHAR_PTR", 0); }
#line 1696 "parser.tab.c"
    break;

  case 30: /* block: BEGIN_T inner_block END_T  */
#line 272 "parser.y"
                              { (yyval.node) = (yyvsp[-1].node); }
#line 1702 "parser.tab.c"
    break;

  case 31: /* block: BEGIN_T END_T  */
#line 273 "parser.y"
                  { (yyval.node) = create_node("BLOCK", 0); }
#line 1708 "parser.tab.c"
    break;

  case 32: /* body: VAR optional_var_list block  */
#line 277 "parser.y"
                                { (yyval.node) = create_node("VAR_BLOCK", 2, (yyvsp[-1].node), (yyvsp[0].node)); }
#line 1714 "parser.tab.c"
    break;

  case 33: /* body: block  */
#line 278 "parser.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1720 "parser.tab.c"
    break;

  case 34: /* inner_block: nested_func inner_block  */
#line 282 "parser.y"
                            {
      if ((yyvsp[0].node) == NULL)
          (yyval.node) = create_node("BLOCK", 1, (yyvsp[-1].node));
      else {
          Node* merged = create_node("BLOCK", (yyvsp[0].node)->child_count + 1);
          merged->children[0] = (yyvsp[-1].node);
          for (int i = 0; i < (yyvsp[0].node)->child_count; i++)
              merged->children[i+1] = (yyvsp[0].node)->children[i];
          (yyval.node) = merged;
      }
  }
#line 1736 "parser.tab.c"
    break;

  case 35: /* inner_block: stmts inner_block  */
#line 293 "parser.y"
                      {
      if ((yyvsp[0].node) == NULL)
          (yyval.node) = create_node("BLOCK", 1, (yyvsp[-1].node));
      else {
          Node* merged = create_node("BLOCK", (yyvsp[0].node)->child_count + 1);
          merged->children[0] = (yyvsp[-1].node);
          for (int i = 0; i < (yyvsp[0].node)->child_count; i++)
              merged->children[i+1] = (yyvsp[0].node)->children[i];
          (yyval.node) = merged;
      }
  }
#line 1752 "parser.tab.c"
    break;

  case 36: /* inner_block: nested_func  */
#line 304 "parser.y"
                { (yyval.node) = create_node("BLOCK", 1, (yyvsp[0].node)); }
#line 1758 "parser.tab.c"
    break;

  case 37: /* inner_block: stmts  */
#line 305 "parser.y"
          { (yyval.node) = create_node("BLOCK", 1, (yyvsp[0].node)); }
#line 1764 "parser.tab.c"
    break;

  case 38: /* inner_block: %empty  */
#line 306 "parser.y"
                { (yyval.node) = create_node("BLOCK", 0); }
#line 1770 "parser.tab.c"
    break;

  case 39: /* stmts: stmt  */
#line 310 "parser.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1776 "parser.tab.c"
    break;

  case 40: /* stmts: stmt stmts  */
#line 311 "parser.y"
               {
      if (strcmp((yyvsp[0].node)->name, "BLOCK") == 0) {
          Node* merged = create_node("BLOCK", (yyvsp[0].node)->child_count + 1);
          merged->children[0] = (yyvsp[-1].node);
          for (int i = 0; i < (yyvsp[0].node)->child_count; i++)
              merged->children[i+1] = (yyvsp[0].node)->children[i];
          (yyval.node) = merged;
      } else {
          (yyval.node) = create_node("BLOCK", 2, (yyvsp[-1].node), (yyvsp[0].node));
      }
  }
#line 1792 "parser.tab.c"
    break;

  case 41: /* stmt: RETURN expr ';'  */
#line 325 "parser.y"
                    {
      current_func_has_return = 1;
      if (!check_return_type((yyvsp[-1].node))) {
          yyerror("Return type mismatch or returning from procedure");
      }
      (yyval.node) = create_node("RET", 1, (yyvsp[-1].node));
  }
#line 1804 "parser.tab.c"
    break;

  case 42: /* stmt: RETURN ';'  */
#line 332 "parser.y"
               {
      current_func_has_return = 1;
      if (strlen(current_return_type) > 0)
          yyerror("Function with return type must return a value");
      (yyval.node) = create_node("RET", 0);
  }
#line 1815 "parser.tab.c"
    break;

  case 43: /* stmt: IDENTIFIER ASSIGN expr ';'  */
#line 338 "parser.y"
                               {
      (yyval.node) = create_node("=", 2, create_node((yyvsp[-3].str), 0), (yyvsp[-1].node));
  }
#line 1823 "parser.tab.c"
    break;

  case 44: /* stmt: IDENTIFIER ASSIGN CALL IDENTIFIER '(' args ')' ';'  */
#line 341 "parser.y"
                                                       {
      Node* call_node = create_node("CALL", 2, create_node((yyvsp[-4].str), 0), (yyvsp[-2].node));
      (yyval.node) = create_node("=", 2, create_node((yyvsp[-7].str), 0), call_node);
  }
#line 1832 "parser.tab.c"
    break;

  case 45: /* stmt: CALL IDENTIFIER '(' args ')' ';'  */
#line 345 "parser.y"
                                     {
      (yyval.node) = create_node("CALL", 2, create_node((yyvsp[-4].str), 0), (yyvsp[-2].node));
  }
#line 1840 "parser.tab.c"
    break;

  case 46: /* stmt: IDENTIFIER '[' expr ']' ASSIGN expr ';'  */
#line 348 "parser.y"
                                            {
      Node* arr_elem = create_node("ARRAY_ELEM", 2, create_node((yyvsp[-6].str), 0), (yyvsp[-4].node));
      (yyval.node) = create_node("=", 2, arr_elem, (yyvsp[-1].node));
  }
#line 1849 "parser.tab.c"
    break;

  case 47: /* stmt: MUL IDENTIFIER ASSIGN expr ';'  */
#line 352 "parser.y"
                                   {
      (yyval.node) = create_node("= *", 2, create_node((yyvsp[-3].str), 0), (yyvsp[-1].node));
  }
#line 1857 "parser.tab.c"
    break;

  case 48: /* stmt: IF expr ':' body ELSE ':' body  */
#line 355 "parser.y"
                                   {
      (yyval.node) = create_node("IF-ELSE", 3, (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[0].node));
  }
#line 1865 "parser.tab.c"
    break;

  case 49: /* stmt: IF expr ':' body ELIF expr ':' body ELSE ':' body  */
#line 358 "parser.y"
                                                      {
      (yyval.node) = create_node("IF-ELIF-ELSE", 5, (yyvsp[-9].node), (yyvsp[-7].node), (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[0].node));
  }
#line 1873 "parser.tab.c"
    break;

  case 50: /* stmt: IF expr ':' body ELIF expr ':' body  */
#line 361 "parser.y"
                                        {
      (yyval.node) = create_node("IF-ELIF", 4, (yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
  }
#line 1881 "parser.tab.c"
    break;

  case 51: /* stmt: IF expr ':' body  */
#line 364 "parser.y"
                     {
      (yyval.node) = create_node("IF", 2, (yyvsp[-2].node), (yyvsp[0].node));
  }
#line 1889 "parser.tab.c"
    break;

  case 52: /* stmt: WHILE expr ':' body  */
#line 367 "parser.y"
                        {
      (yyval.node) = create_node("WHILE", 2, (yyvsp[-2].node), (yyvsp[0].node));
  }
#line 1897 "parser.tab.c"
    break;

  case 53: /* stmt: DO ':' body WHILE expr ';'  */
#line 370 "parser.y"
                               {
      (yyval.node) = create_node("DO-WHILE", 2, (yyvsp[-3].node), (yyvsp[-1].node));
  }
#line 1905 "parser.tab.c"
    break;

  case 54: /* stmt: FOR '(' expr ';' expr ';' expr ')' ':' body  */
#line 373 "parser.y"
                                                {
      Node* cond = create_node("FOR_COND", 3, (yyvsp[-7].node), (yyvsp[-5].node), (yyvsp[-3].node));
      (yyval.node) = create_node("FOR", 2, cond, (yyvsp[0].node));
  }
#line 1914 "parser.tab.c"
    break;

  case 55: /* stmt: VAR optional_var_list block  */
#line 377 "parser.y"
                                {
      (yyval.node) = create_node("BODY", 2, (yyvsp[-1].node), (yyvsp[0].node));
  }
#line 1922 "parser.tab.c"
    break;

  case 56: /* stmt: block  */
#line 380 "parser.y"
          {
      (yyval.node) = (yyvsp[0].node);
  }
#line 1930 "parser.tab.c"
    break;

  case 57: /* stmt: VAR optional_var_list stmt  */
#line 383 "parser.y"
                               {
      (yyval.node) = create_node("VAR_BLOCK", 2, (yyvsp[-1].node), (yyvsp[0].node));
  }
#line 1938 "parser.tab.c"
    break;

  case 58: /* var_decls: VAR optional_var_list  */
#line 390 "parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1944 "parser.tab.c"
    break;

  case 59: /* var_decls: %empty  */
#line 391 "parser.y"
                { (yyval.node) = create_node("BLOCK", 0); }
#line 1950 "parser.tab.c"
    break;

  case 60: /* optional_var_list: var_decl_list  */
#line 395 "parser.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 1956 "parser.tab.c"
    break;

  case 61: /* optional_var_list: %empty  */
#line 396 "parser.y"
                { (yyval.node) = create_node("BLOCK", 0); }
#line 1962 "parser.tab.c"
    break;

  case 62: /* var_decl_list: var_decl_list var_single_decl  */
#line 400 "parser.y"
                                  {
      Node* merged = create_node("BLOCK", (yyvsp[-1].node)->child_count + 1);
      for (int i = 0; i < (yyvsp[-1].node)->child_count; i++)
          merged->children[i] = (yyvsp[-1].node)->children[i];
      merged->children[(yyvsp[-1].node)->child_count] = (yyvsp[0].node);
      (yyval.node) = merged;
  }
#line 1974 "parser.tab.c"
    break;

  case 63: /* var_decl_list: var_single_decl  */
#line 407 "parser.y"
                    { (yyval.node) = create_node("BLOCK", 1, (yyvsp[0].node)); }
#line 1980 "parser.tab.c"
    break;

  case 64: /* var_single_decl: TYPE type ':' IDENTIFIER ';'  */
#line 411 "parser.y"
                                 {
      char temp[100];
      sprintf(temp, "%s %s", (yyvsp[-3].node)->name, (yyvsp[-1].str));
      (yyval.node) = create_node(temp, 0);
  }
#line 1990 "parser.tab.c"
    break;

  case 65: /* var_single_decl: TYPE type ':' IDENTIFIER ':' expr ';'  */
#line 416 "parser.y"
                                          {
      char temp[100];
      sprintf(temp, "%s %s", (yyvsp[-5].node)->name, (yyvsp[-3].str));
      Node* var_node = create_node(temp, 0);
      (yyval.node) = create_node("=", 2, var_node, (yyvsp[-1].node));
  }
#line 2001 "parser.tab.c"
    break;

  case 66: /* var_single_decl: TYPE BOOL ':' var_init_list ';'  */
#line 422 "parser.y"
                                    {
      (yyval.node) = (yyvsp[-1].node);
  }
#line 2009 "parser.tab.c"
    break;

  case 67: /* var_single_decl: TYPE type ':' string_decl_list ';'  */
#line 425 "parser.y"
                                       {
      (yyval.node) = (yyvsp[-1].node);
  }
#line 2017 "parser.tab.c"
    break;

  case 68: /* var_init_list: IDENTIFIER ':' expr  */
#line 432 "parser.y"
                        {
      char temp[100];
      sprintf(temp, "BOOL %s", (yyvsp[-2].str));
      Node* var_node = create_node(temp, 0);
      (yyval.node) = create_node("=", 2, var_node, (yyvsp[0].node));
  }
#line 2028 "parser.tab.c"
    break;

  case 69: /* var_init_list: IDENTIFIER ':' expr ',' var_init_list  */
#line 438 "parser.y"
                                          {
      char temp[100];
      sprintf(temp, "BOOL %s", (yyvsp[-4].str));
      Node* var_node = create_node(temp, 0);
      Node* assign_node = create_node("=", 2, var_node, (yyvsp[-2].node));
      Node* merged = create_node("BLOCK", (yyvsp[0].node)->child_count + 1);
      merged->children[0] = assign_node;
      for (int i = 0; i < (yyvsp[0].node)->child_count; i++)
          merged->children[i+1] = (yyvsp[0].node)->children[i];
      (yyval.node) = merged;
  }
#line 2044 "parser.tab.c"
    break;

  case 70: /* string_decl_list: string_decl_list ',' string_decl  */
#line 453 "parser.y"
                                     {
      Node* merged = create_node("BLOCK", (yyvsp[-2].node)->child_count + 1);
      for (int i = 0; i < (yyvsp[-2].node)->child_count; i++)
          merged->children[i] = (yyvsp[-2].node)->children[i];
      merged->children[(yyvsp[-2].node)->child_count] = (yyvsp[0].node);
      (yyval.node) = merged;
  }
#line 2056 "parser.tab.c"
    break;

  case 71: /* string_decl_list: string_decl  */
#line 460 "parser.y"
                {
      (yyval.node) = create_node("BLOCK", 1, (yyvsp[0].node));
  }
#line 2064 "parser.tab.c"
    break;

  case 72: /* string_decl: IDENTIFIER '[' INT_LITERAL ']'  */
#line 466 "parser.y"
                                   {
      char temp[100];
      sprintf(temp, "STR %s[%s]", (yyvsp[-3].str), (yyvsp[-1].str));
      (yyval.node) = create_node(temp, 0);
  }
#line 2074 "parser.tab.c"
    break;

  case 73: /* string_decl: IDENTIFIER '[' INT_LITERAL ']' ':' STRING_LITERAL  */
#line 471 "parser.y"
                                                      {
      char temp[100];
      sprintf(temp, "STR %s[%s]:%s", (yyvsp[-5].str), (yyvsp[-3].str), (yyvsp[0].str));
      (yyval.node) = create_node(temp, 0);
  }
#line 2084 "parser.tab.c"
    break;

  case 74: /* expr: expr ADD expr  */
#line 480 "parser.y"
                  { (yyval.node) = create_node("+", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2090 "parser.tab.c"
    break;

  case 75: /* expr: expr SUB expr  */
#line 481 "parser.y"
                  { (yyval.node) = create_node("-", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2096 "parser.tab.c"
    break;

  case 76: /* expr: expr MUL expr  */
#line 482 "parser.y"
                  { (yyval.node) = create_node("*", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2102 "parser.tab.c"
    break;

  case 77: /* expr: expr DIV expr  */
#line 483 "parser.y"
                  { (yyval.node) = create_node("/", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2108 "parser.tab.c"
    break;

  case 78: /* expr: expr EQ expr  */
#line 484 "parser.y"
                 { (yyval.node) = create_node("==", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2114 "parser.tab.c"
    break;

  case 79: /* expr: expr NE expr  */
#line 485 "parser.y"
                 { (yyval.node) = create_node("!=", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2120 "parser.tab.c"
    break;

  case 80: /* expr: expr GT expr  */
#line 486 "parser.y"
                 { (yyval.node) = create_node(">", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2126 "parser.tab.c"
    break;

  case 81: /* expr: expr GE expr  */
#line 487 "parser.y"
                 { (yyval.node) = create_node(">=", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2132 "parser.tab.c"
    break;

  case 82: /* expr: expr LT expr  */
#line 488 "parser.y"
                 { (yyval.node) = create_node("<", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2138 "parser.tab.c"
    break;

  case 83: /* expr: expr LE expr  */
#line 489 "parser.y"
                 { (yyval.node) = create_node("<=", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2144 "parser.tab.c"
    break;

  case 84: /* expr: expr AND expr  */
#line 490 "parser.y"
                  { (yyval.node) = create_node("AND", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2150 "parser.tab.c"
    break;

  case 85: /* expr: expr OR expr  */
#line 491 "parser.y"
                 { (yyval.node) = create_node("OR", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2156 "parser.tab.c"
    break;

  case 86: /* expr: NOT expr  */
#line 492 "parser.y"
             { (yyval.node) = create_node("NOT", 1, (yyvsp[0].node)); }
#line 2162 "parser.tab.c"
    break;

  case 87: /* expr: '(' expr ')'  */
#line 493 "parser.y"
                 { (yyval.node) = (yyvsp[-1].node); }
#line 2168 "parser.tab.c"
    break;

  case 88: /* expr: SUB expr  */
#line 494 "parser.y"
                          { (yyval.node) = create_node("-", 2, create_node("0", 0), (yyvsp[0].node)); }
#line 2174 "parser.tab.c"
    break;

  case 89: /* expr: MUL IDENTIFIER  */
#line 495 "parser.y"
                   { (yyval.node) = create_node("*", 1, create_node((yyvsp[0].str), 0)); }
#line 2180 "parser.tab.c"
    break;

  case 90: /* expr: ADDR IDENTIFIER  */
#line 496 "parser.y"
                    { (yyval.node) = create_node("&", 1, create_node((yyvsp[0].str), 0)); }
#line 2186 "parser.tab.c"
    break;

  case 91: /* expr: MUL '(' expr ')'  */
#line 497 "parser.y"
                     { (yyval.node) = create_node("*", 1, (yyvsp[-1].node)); }
#line 2192 "parser.tab.c"
    break;

  case 92: /* expr: IDENTIFIER  */
#line 498 "parser.y"
               { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 2198 "parser.tab.c"
    break;

  case 93: /* expr: IDENTIFIER '(' args ')'  */
#line 499 "parser.y"
                            { (yyval.node) = create_node("CALL", 2, create_node((yyvsp[-3].str), 0), (yyvsp[-1].node)); }
#line 2204 "parser.tab.c"
    break;

  case 94: /* expr: IDENTIFIER '[' expr ']'  */
#line 500 "parser.y"
                            { (yyval.node) = create_node("ARRAY_ELEM", 2, create_node((yyvsp[-3].str), 0), (yyvsp[-1].node)); }
#line 2210 "parser.tab.c"
    break;

  case 95: /* expr: INT_LITERAL  */
#line 501 "parser.y"
                { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 2216 "parser.tab.c"
    break;

  case 96: /* expr: REAL_LITERAL  */
#line 502 "parser.y"
                 { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 2222 "parser.tab.c"
    break;

  case 97: /* expr: CHAR_LITERAL  */
#line 503 "parser.y"
                 { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 2228 "parser.tab.c"
    break;

  case 98: /* expr: STRING_LITERAL  */
#line 504 "parser.y"
                   { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 2234 "parser.tab.c"
    break;

  case 99: /* expr: TRUE  */
#line 505 "parser.y"
         { (yyval.node) = create_node("TRUE", 0); }
#line 2240 "parser.tab.c"
    break;

  case 100: /* expr: FALSE  */
#line 506 "parser.y"
          { (yyval.node) = create_node("FALSE", 0); }
#line 2246 "parser.tab.c"
    break;

  case 101: /* expr: NULL_T  */
#line 507 "parser.y"
           { (yyval.node) = create_node("NULL", 0); }
#line 2252 "parser.tab.c"
    break;

  case 102: /* expr: PIPE_SYMBOL IDENTIFIER PIPE_SYMBOL  */
#line 508 "parser.y"
                                       { (yyval.node) = create_node("LEN", 1, create_node((yyvsp[-1].str), 0)); }
#line 2258 "parser.tab.c"
    break;

  case 103: /* args: expr  */
#line 513 "parser.y"
         { (yyval.node) = create_node("ARGS", 1, (yyvsp[0].node)); }
#line 2264 "parser.tab.c"
    break;

  case 104: /* args: expr ',' args  */
#line 514 "parser.y"
                  {
      if (strcmp((yyvsp[0].node)->name, "ARGS") == 0) {
          Node* merged = create_node("ARGS", (yyvsp[0].node)->child_count + 1);
          merged->children[0] = (yyvsp[-2].node);
          for (int i = 0; i < (yyvsp[0].node)->child_count; i++)
              merged->children[i+1] = (yyvsp[0].node)->children[i];
          (yyval.node) = merged;
      } else {
          (yyval.node) = create_node("ARGS", 2, (yyvsp[-2].node), (yyvsp[0].node));
      }
  }
#line 2280 "parser.tab.c"
    break;

  case 105: /* args: %empty  */
#line 525 "parser.y"
                { (yyval.node) = create_node("ARGS NONE", 0); }
#line 2286 "parser.tab.c"
    break;


#line 2290 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 528 "parser.y"


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

