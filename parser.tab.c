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
#include "ast.h"
#include "symbol_table.h"

int yylex(void);
int yyerror(const char* s) {
    fprintf(stderr, "Syntax error: %s\n", s);
    return 1;
}

Node* root;
int main_defined = 0;


#line 89 "parser.tab.c"

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
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_TYPE = 4,                       /* TYPE  */
  YYSYMBOL_STRING_LITERAL = 5,             /* STRING_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 6,               /* CHAR_LITERAL  */
  YYSYMBOL_BOOL_LITERAL = 7,               /* BOOL_LITERAL  */
  YYSYMBOL_INT_LITERAL = 8,                /* INT_LITERAL  */
  YYSYMBOL_REAL_LITERAL = 9,               /* REAL_LITERAL  */
  YYSYMBOL_DEF = 10,                       /* DEF  */
  YYSYMBOL_RETURNS = 11,                   /* RETURNS  */
  YYSYMBOL_RETURN = 12,                    /* RETURN  */
  YYSYMBOL_IF = 13,                        /* IF  */
  YYSYMBOL_ELSE = 14,                      /* ELSE  */
  YYSYMBOL_ELIF = 15,                      /* ELIF  */
  YYSYMBOL_WHILE = 16,                     /* WHILE  */
  YYSYMBOL_FOR = 17,                       /* FOR  */
  YYSYMBOL_DO = 18,                        /* DO  */
  YYSYMBOL_VAR = 19,                       /* VAR  */
  YYSYMBOL_BEGIN_T = 20,                   /* BEGIN_T  */
  YYSYMBOL_END_T = 21,                     /* END_T  */
  YYSYMBOL_CALL = 22,                      /* CALL  */
  YYSYMBOL_NULL_T = 23,                    /* NULL_T  */
  YYSYMBOL_TRUE = 24,                      /* TRUE  */
  YYSYMBOL_FALSE = 25,                     /* FALSE  */
  YYSYMBOL_VOID = 26,                      /* VOID  */
  YYSYMBOL_INT = 27,                       /* INT  */
  YYSYMBOL_REAL = 28,                      /* REAL  */
  YYSYMBOL_CHAR = 29,                      /* CHAR  */
  YYSYMBOL_BOOL = 30,                      /* BOOL  */
  YYSYMBOL_STRING = 31,                    /* STRING  */
  YYSYMBOL_INT_PTR = 32,                   /* INT_PTR  */
  YYSYMBOL_REAL_PTR = 33,                  /* REAL_PTR  */
  YYSYMBOL_CHAR_PTR = 34,                  /* CHAR_PTR  */
  YYSYMBOL_ADD = 35,                       /* ADD  */
  YYSYMBOL_SUB = 36,                       /* SUB  */
  YYSYMBOL_MUL = 37,                       /* MUL  */
  YYSYMBOL_DIV = 38,                       /* DIV  */
  YYSYMBOL_ASSIGN = 39,                    /* ASSIGN  */
  YYSYMBOL_EQ = 40,                        /* EQ  */
  YYSYMBOL_NEQ = 41,                       /* NEQ  */
  YYSYMBOL_LEQ = 42,                       /* LEQ  */
  YYSYMBOL_GEQ = 43,                       /* GEQ  */
  YYSYMBOL_LT = 44,                        /* LT  */
  YYSYMBOL_GT = 45,                        /* GT  */
  YYSYMBOL_AND = 46,                       /* AND  */
  YYSYMBOL_OR = 47,                        /* OR  */
  YYSYMBOL_NOT = 48,                       /* NOT  */
  YYSYMBOL_ADDR = 49,                      /* ADDR  */
  YYSYMBOL_PIPE_SYMBOL = 50,               /* PIPE_SYMBOL  */
  YYSYMBOL_UMINUS = 51,                    /* UMINUS  */
  YYSYMBOL_52_ = 52,                       /* '('  */
  YYSYMBOL_53_ = 53,                       /* ')'  */
  YYSYMBOL_54_ = 54,                       /* ':'  */
  YYSYMBOL_55_ = 55,                       /* ','  */
  YYSYMBOL_56_ = 56,                       /* ';'  */
  YYSYMBOL_YYACCEPT = 57,                  /* $accept  */
  YYSYMBOL_program = 58,                   /* program  */
  YYSYMBOL_funcs = 59,                     /* funcs  */
  YYSYMBOL_func = 60,                      /* func  */
  YYSYMBOL_func_header = 61,               /* func_header  */
  YYSYMBOL_parameters = 62,                /* parameters  */
  YYSYMBOL_parameter = 63,                 /* parameter  */
  YYSYMBOL_ret_type = 64,                  /* ret_type  */
  YYSYMBOL_type_name = 65,                 /* type_name  */
  YYSYMBOL_body = 66,                      /* body  */
  YYSYMBOL_block = 67,                     /* block  */
  YYSYMBOL_inner_block = 68,               /* inner_block  */
  YYSYMBOL_stmts = 69,                     /* stmts  */
  YYSYMBOL_stmt = 70,                      /* stmt  */
  YYSYMBOL_call_stmt = 71,                 /* call_stmt  */
  YYSYMBOL_for_expr = 72,                  /* for_expr  */
  YYSYMBOL_optional_var_list = 73,         /* optional_var_list  */
  YYSYMBOL_var_decl_list = 74,             /* var_decl_list  */
  YYSYMBOL_var_single_decl = 75,           /* var_single_decl  */
  YYSYMBOL_args = 76,                      /* args  */
  YYSYMBOL_expr = 77                       /* expr  */
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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYLAST   327

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  137

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
      52,    53,     2,     2,    55,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    54,    56,
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
       0,    52,    52,    56,    57,    58,    62,    71,   109,   144,
     145,   149,   158,   162,   163,   164,   165,   166,   167,   168,
     169,   173,   174,   178,   187,   188,   192,   193,   197,   198,
     201,   204,   207,   212,   217,   220,   225,   230,   236,   237,
     241,   242,   246,   247,   251,   283,   306,   307,   308,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331
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
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER", "TYPE",
  "STRING_LITERAL", "CHAR_LITERAL", "BOOL_LITERAL", "INT_LITERAL",
  "REAL_LITERAL", "DEF", "RETURNS", "RETURN", "IF", "ELSE", "ELIF",
  "WHILE", "FOR", "DO", "VAR", "BEGIN_T", "END_T", "CALL", "NULL_T",
  "TRUE", "FALSE", "VOID", "INT", "REAL", "CHAR", "BOOL", "STRING",
  "INT_PTR", "REAL_PTR", "CHAR_PTR", "ADD", "SUB", "MUL", "DIV", "ASSIGN",
  "EQ", "NEQ", "LEQ", "GEQ", "LT", "GT", "AND", "OR", "NOT", "ADDR",
  "PIPE_SYMBOL", "UMINUS", "'('", "')'", "':'", "','", "';'", "$accept",
  "program", "funcs", "func", "func_header", "parameters", "parameter",
  "ret_type", "type_name", "body", "block", "inner_block", "stmts", "stmt",
  "call_stmt", "for_expr", "optional_var_list", "var_decl_list",
  "var_single_decl", "args", "expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-88)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -3,    18,    14,   -88,    -3,    -1,   -20,   -88,   -88,    34,
      76,   -88,   -88,    13,     1,    24,    34,   -88,   -88,   -88,
     -88,   -88,   -88,     0,     0,     0,     2,    50,   -88,   -88,
       0,     0,    48,   -88,    76,    16,    94,   -88,   -88,   -88,
     -88,   -88,   -88,   -88,   -88,    19,    17,    20,    22,    35,
      23,   -88,   -88,   116,   203,   223,     0,    28,   -88,   263,
     -88,   -88,   -88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -88,    72,    33,   114,    87,     0,
      88,   -88,    24,    24,    38,   282,     0,   -88,    11,    11,
     -88,   -88,   282,    79,    79,   -25,   -25,   -25,   -25,   114,
      84,   -88,   -88,   138,    57,    36,   -88,     0,    44,   182,
     -88,   -88,   114,   -88,     0,    49,     0,    46,   -88,     0,
     -88,   160,    24,   243,     0,   -88,   -88,   -88,    24,    51,
      91,    55,    56,    24,    24,   -88,   -88
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,     0,     2,     3,     0,     0,     1,     4,    41,
      25,     6,    22,     0,     0,     0,    40,    42,    62,    66,
      65,    63,    64,     0,     0,     0,     0,     0,    67,    68,
       0,     0,     0,    24,    26,     0,     0,    20,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     9,     0,     0,
       0,    21,    43,     0,     0,     0,    39,     0,    54,     0,
      23,    27,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    28,     0,     0,     0,     0,     0,
       0,    29,     0,     0,     0,    38,    48,    61,    50,    51,
      52,    53,    49,    55,    56,    57,    58,    59,    60,     0,
       0,    10,    11,     0,     0,    30,    34,    39,     0,    46,
       8,    12,     0,    44,     0,     0,     0,     0,    37,    48,
       7,     0,     0,     0,    39,    47,    45,    31,     0,     0,
      32,     0,     0,     0,     0,    35,    33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -88,   -88,   121,   -88,   -88,    43,   -88,    15,   -13,   -88,
     -15,   -88,    92,   -88,   -88,   -87,   -88,   -88,   129,    30,
      -8
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,     4,     5,    46,    47,   110,    48,    11,
      12,    32,    33,    34,    35,    84,    15,    16,    17,   108,
      85
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      51,    50,    36,    18,    49,    19,    20,     1,    21,    22,
      63,    64,    65,    66,     7,    53,    54,    55,     9,    10,
     117,     6,    58,    59,    28,    29,    36,    37,    38,    39,
      40,    41,    13,    42,    43,    44,    30,   129,    14,    37,
      38,    39,    40,    41,    10,    42,    43,    44,    65,    66,
     115,   116,    31,    57,    56,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    45,   105,   106,    60,
      76,   103,    62,    75,    79,    77,    78,    80,   109,    18,
      86,    19,    20,    99,    21,    22,   111,   100,    23,    24,
     102,   104,    25,    26,   107,   112,   114,   118,    27,   111,
      28,    29,   124,   122,   131,   132,   121,   127,   123,   133,
     134,   109,    30,   130,    63,    64,    65,    66,   135,   136,
     101,    70,    71,    72,    73,     8,    61,   120,    31,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      37,    38,    39,    40,    41,    52,    42,    43,    44,   125,
      74,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    81,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   126,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   119,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,     0,
       0,     0,     0,     0,     0,     0,     0,    82,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,     0,
       0,     0,     0,     0,     0,     0,     0,   128,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,     0,
       0,     0,     0,     0,     0,     0,    87,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73
};

static const yytype_int16 yycheck[] =
{
      15,    14,    10,     3,     3,     5,     6,    10,     8,     9,
      35,    36,    37,    38,     0,    23,    24,    25,    19,    20,
     107,     3,    30,    31,    24,    25,    34,    26,    27,    28,
      29,    30,    52,    32,    33,    34,    36,   124,     4,    26,
      27,    28,    29,    30,    20,    32,    33,    34,    37,    38,
      14,    15,    52,     3,    52,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    53,    82,    83,    21,
      53,    79,    56,    54,    39,    55,    54,    54,    86,     3,
      52,     5,     6,    11,     8,     9,    99,    54,    12,    13,
       3,     3,    16,    17,    56,    11,    39,    53,    22,   112,
      24,    25,    56,    54,    53,    14,   114,   122,   116,    54,
      54,   119,    36,   128,    35,    36,    37,    38,   133,   134,
      77,    42,    43,    44,    45,     4,    34,   112,    52,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      26,    27,    28,    29,    30,    16,    32,    33,    34,   119,
      56,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    10,    58,    59,    60,    61,     3,     0,    59,    19,
      20,    66,    67,    52,     4,    73,    74,    75,     3,     5,
       6,     8,     9,    12,    13,    16,    17,    22,    24,    25,
      36,    52,    68,    69,    70,    71,    77,    26,    27,    28,
      29,    30,    32,    33,    34,    53,    62,    63,    65,     3,
      65,    67,    75,    77,    77,    77,    52,     3,    77,    77,
      21,    69,    56,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    56,    54,    53,    55,    54,    39,
      54,    56,    54,    54,    72,    77,    52,    53,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    11,
      54,    62,     3,    77,     3,    67,    67,    56,    76,    77,
      64,    65,    11,    56,    39,    14,    15,    72,    53,    55,
      64,    77,    54,    77,    56,    76,    56,    67,    54,    72,
      67,    53,    14,    54,    54,    67,    67
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    59,    59,    59,    60,    61,    61,    62,
      62,    63,    64,    65,    65,    65,    65,    65,    65,    65,
      65,    66,    66,    67,    68,    68,    69,    69,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    71,    72,    72,
      73,    73,    74,    74,    75,    75,    76,    76,    76,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     0,     2,     8,     7,     1,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     3,     1,     0,     1,     2,     2,     3,
       4,     7,     8,    11,     4,    10,     2,     5,     1,     0,
       1,     0,     1,     2,     5,     7,     1,     3,     0,     3,
       3,     3,     3,     3,     2,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1
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

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */


/* User initialization code.  */
#line 21 "parser.y"
{
    init_symbol_table();
}

#line 1191 "parser.tab.c"

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: funcs  */
#line 52 "parser.y"
            { root = create_node("CODE", 1, (yyvsp[0].node)); }
#line 1404 "parser.tab.c"
    break;

  case 3: /* funcs: func  */
#line 56 "parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1410 "parser.tab.c"
    break;

  case 4: /* funcs: func funcs  */
#line 57 "parser.y"
                 { (yyval.node) = create_node("FUNC", 2, (yyvsp[-1].node), (yyvsp[0].node)); }
#line 1416 "parser.tab.c"
    break;

  case 5: /* funcs: %empty  */
#line 58 "parser.y"
                  { (yyval.node) = NULL; }
#line 1422 "parser.tab.c"
    break;

  case 6: /* func: func_header body  */
#line 62 "parser.y"
                       {
        enter_scope();  // התחלת גוף הפונקציה
        (yyval.node) = create_node((yyvsp[-1].node)->name, 3, (yyvsp[-1].node)->children[0], (yyvsp[-1].node)->children[1], create_node("BODY", 1, (yyvsp[0].node)));
        exit_scope();   // סיום גוף הפונקציה
    }
#line 1432 "parser.tab.c"
    break;

  case 7: /* func_header: DEF IDENTIFIER '(' parameters ')' ':' RETURNS ret_type  */
#line 71 "parser.y"
                                                             {
        (yyval.node) = create_node((yyvsp[-6].str), 2, (yyvsp[-4].node), (yyvsp[0].node));

        VarType ret;
        if (strcmp((yyvsp[0].node)->name, "INT") == 0) ret = TYPE_INT;
        else if (strcmp((yyvsp[0].node)->name, "REAL") == 0) ret = TYPE_REAL;
        else if (strcmp((yyvsp[0].node)->name, "CHAR") == 0) ret = TYPE_CHAR;
        else if (strcmp((yyvsp[0].node)->name, "BOOL") == 0) ret = TYPE_BOOL;
        else if (strcmp((yyvsp[0].node)->name, "INT_PTR") == 0) ret = TYPE_INT_PTR;
        else if (strcmp((yyvsp[0].node)->name, "REAL_PTR") == 0) ret = TYPE_REAL_PTR;
        else if (strcmp((yyvsp[0].node)->name, "CHAR_PTR") == 0) ret = TYPE_CHAR_PTR;
        else ret = TYPE_VOID;

        if (!insert_symbol((yyvsp[-6].str), FUNC_SYM, ret, -1)) {
            semantic_error("Function already defined in this scope", (yylsp[-6]).first_line);
            YYABORT;
        }

        if (strcmp((yyvsp[-6].str), "_main_") == 0) {
            if (main_defined) {
                semantic_error("Multiple definitions of _main_", (yylsp[-6]).first_line);
                YYABORT;
            }

            if (ret != TYPE_VOID) {
                semantic_error("_main_ must not return a value", (yylsp[-6]).first_line);
                YYABORT;
            }

            if ((yyvsp[-4].node)->num_children > 0) {
                semantic_error("_main_ must not have parameters", (yylsp[-6]).first_line);
                YYABORT;
            }

            main_defined = 1;  // ✅ אם כל התנאים עברו
        }
    }
#line 1474 "parser.tab.c"
    break;

  case 8: /* func_header: DEF IDENTIFIER '(' ')' ':' RETURNS ret_type  */
#line 109 "parser.y"
                                                  {
        (yyval.node) = create_node((yyvsp[-5].str), 2, create_node("PARS", 0), (yyvsp[0].node));

        VarType ret;
        if (strcmp((yyvsp[0].node)->name, "INT") == 0) ret = TYPE_INT;
        else if (strcmp((yyvsp[0].node)->name, "REAL") == 0) ret = TYPE_REAL;
        else if (strcmp((yyvsp[0].node)->name, "CHAR") == 0) ret = TYPE_CHAR;
        else if (strcmp((yyvsp[0].node)->name, "BOOL") == 0) ret = TYPE_BOOL;
        else if (strcmp((yyvsp[0].node)->name, "INT_PTR") == 0) ret = TYPE_INT_PTR;
        else if (strcmp((yyvsp[0].node)->name, "REAL_PTR") == 0) ret = TYPE_REAL_PTR;
        else if (strcmp((yyvsp[0].node)->name, "CHAR_PTR") == 0) ret = TYPE_CHAR_PTR;
        else ret = TYPE_VOID;

        if (!insert_symbol((yyvsp[-5].str), FUNC_SYM, ret, -1)) {
            semantic_error("Function already defined in this scope", (yylsp[-5]).first_line);
            YYABORT;
        }

        if (strcmp((yyvsp[-5].str), "_main_") == 0) {
            if (main_defined) {
                semantic_error("Multiple definitions of _main_", (yylsp[-5]).first_line);
                YYABORT;
            }

            if (ret != TYPE_VOID) {
                semantic_error("_main_ must not return a value", (yylsp[-5]).first_line);
                YYABORT;
            }

            main_defined = 1;
        }
    }
#line 1511 "parser.tab.c"
    break;

  case 9: /* parameters: parameter  */
#line 144 "parser.y"
                                               { (yyval.node) = create_node("PARS", 1, (yyvsp[0].node)); }
#line 1517 "parser.tab.c"
    break;

  case 10: /* parameters: parameter ',' parameters  */
#line 145 "parser.y"
                                              { (yyval.node) = create_node("PARS", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1523 "parser.tab.c"
    break;

  case 11: /* parameter: type_name ':' IDENTIFIER  */
#line 149 "parser.y"
                                              {
        char temp[128];
        sprintf(temp, "%s:%s", (yyvsp[-2].node)->name, (yyvsp[0].str));
        (yyval.node) = create_node(strdup(temp), 0);
    }
#line 1533 "parser.tab.c"
    break;

  case 12: /* ret_type: type_name  */
#line 158 "parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1539 "parser.tab.c"
    break;

  case 13: /* type_name: INT  */
#line 162 "parser.y"
                { (yyval.node) = create_node("INT", 0); }
#line 1545 "parser.tab.c"
    break;

  case 14: /* type_name: REAL  */
#line 163 "parser.y"
                { (yyval.node) = create_node("REAL", 0); }
#line 1551 "parser.tab.c"
    break;

  case 15: /* type_name: CHAR  */
#line 164 "parser.y"
                { (yyval.node) = create_node("CHAR", 0); }
#line 1557 "parser.tab.c"
    break;

  case 16: /* type_name: BOOL  */
#line 165 "parser.y"
                { (yyval.node) = create_node("BOOL", 0); }
#line 1563 "parser.tab.c"
    break;

  case 17: /* type_name: INT_PTR  */
#line 166 "parser.y"
                { (yyval.node) = create_node("INT_PTR", 0); }
#line 1569 "parser.tab.c"
    break;

  case 18: /* type_name: REAL_PTR  */
#line 167 "parser.y"
                { (yyval.node) = create_node("REAL_PTR", 0); }
#line 1575 "parser.tab.c"
    break;

  case 19: /* type_name: CHAR_PTR  */
#line 168 "parser.y"
                { (yyval.node) = create_node("CHAR_PTR", 0); }
#line 1581 "parser.tab.c"
    break;

  case 20: /* type_name: VOID  */
#line 169 "parser.y"
                { (yyval.node) = create_node("VOID", 0); }
#line 1587 "parser.tab.c"
    break;

  case 21: /* body: VAR optional_var_list block  */
#line 173 "parser.y"
                                  { (yyval.node) = create_node("VAR_BLOCK", 2, (yyvsp[-1].node), (yyvsp[0].node)); }
#line 1593 "parser.tab.c"
    break;

  case 22: /* body: block  */
#line 174 "parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1599 "parser.tab.c"
    break;

  case 23: /* block: BEGIN_T inner_block END_T  */
#line 178 "parser.y"
                                {
        enter_scope();
        (yyval.node) = (yyvsp[-1].node);
        exit_scope();
    }
#line 1609 "parser.tab.c"
    break;

  case 24: /* inner_block: stmts  */
#line 187 "parser.y"
            { (yyval.node) = create_node("BLOCK", 1, (yyvsp[0].node)); }
#line 1615 "parser.tab.c"
    break;

  case 25: /* inner_block: %empty  */
#line 188 "parser.y"
                  { (yyval.node) = create_node("BLOCK", 0); }
#line 1621 "parser.tab.c"
    break;

  case 26: /* stmts: stmt  */
#line 192 "parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1627 "parser.tab.c"
    break;

  case 27: /* stmts: stmt stmts  */
#line 193 "parser.y"
                 { (yyval.node) = create_node("BLOCK", 2, (yyvsp[-1].node), (yyvsp[0].node)); }
#line 1633 "parser.tab.c"
    break;

  case 28: /* stmt: expr ';'  */
#line 197 "parser.y"
                                                            { (yyval.node) = (yyvsp[-1].node); }
#line 1639 "parser.tab.c"
    break;

  case 29: /* stmt: RETURN expr ';'  */
#line 198 "parser.y"
                                                            { (yyval.node) = create_node("RETURN", 1, (yyvsp[-1].node)); }
#line 1645 "parser.tab.c"
    break;

  case 30: /* stmt: IF expr ':' block  */
#line 201 "parser.y"
                                                            { (yyval.node) = create_node("IF", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1651 "parser.tab.c"
    break;

  case 31: /* stmt: IF expr ':' block ELSE ':' block  */
#line 204 "parser.y"
                                                            { (yyval.node) = create_node("IF_ELSE", 3, (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[0].node)); }
#line 1657 "parser.tab.c"
    break;

  case 32: /* stmt: IF expr ':' block ELIF expr ':' block  */
#line 207 "parser.y"
                                                            { 
        (yyval.node) = create_node("IF_ELIF", 3, (yyvsp[-6].node), (yyvsp[-4].node), create_node("ELIF", 2, (yyvsp[-2].node), (yyvsp[0].node))); 
    }
#line 1665 "parser.tab.c"
    break;

  case 33: /* stmt: IF expr ':' block ELIF expr ':' block ELSE ':' block  */
#line 212 "parser.y"
                                                           {
        (yyval.node) = create_node("IF_ELIF_ELSE", 4, (yyvsp[-9].node), (yyvsp[-7].node), create_node("ELIF", 2, (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[0].node));
    }
#line 1673 "parser.tab.c"
    break;

  case 34: /* stmt: WHILE expr ':' block  */
#line 217 "parser.y"
                                    { (yyval.node) = create_node("WHILE", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1679 "parser.tab.c"
    break;

  case 35: /* stmt: FOR '(' for_expr ';' for_expr ';' for_expr ')' ':' block  */
#line 220 "parser.y"
                                                               {
        (yyval.node) = create_node("FOR", 4, (yyvsp[-7].node), (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[0].node));
    }
#line 1687 "parser.tab.c"
    break;

  case 36: /* stmt: call_stmt ';'  */
#line 225 "parser.y"
                       { (yyval.node) = (yyvsp[-1].node); }
#line 1693 "parser.tab.c"
    break;

  case 37: /* call_stmt: CALL IDENTIFIER '(' args ')'  */
#line 230 "parser.y"
                                   {
        (yyval.node) = create_node("CALL", 2, create_node((yyvsp[-3].str), 0), (yyvsp[-1].node));
    }
#line 1701 "parser.tab.c"
    break;

  case 38: /* for_expr: expr  */
#line 236 "parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1707 "parser.tab.c"
    break;

  case 39: /* for_expr: %empty  */
#line 237 "parser.y"
                  { (yyval.node) = create_node("EMPTY", 0); }
#line 1713 "parser.tab.c"
    break;

  case 40: /* optional_var_list: var_decl_list  */
#line 241 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1719 "parser.tab.c"
    break;

  case 41: /* optional_var_list: %empty  */
#line 242 "parser.y"
                  { (yyval.node) = create_node("BLOCK", 0); }
#line 1725 "parser.tab.c"
    break;

  case 42: /* var_decl_list: var_single_decl  */
#line 246 "parser.y"
                      { (yyval.node) = create_node("BLOCK", 1, (yyvsp[0].node)); }
#line 1731 "parser.tab.c"
    break;

  case 43: /* var_decl_list: var_decl_list var_single_decl  */
#line 247 "parser.y"
                                    { (yyval.node) = create_node("BLOCK", 2, (yyvsp[-1].node), (yyvsp[0].node)); }
#line 1737 "parser.tab.c"
    break;

  case 44: /* var_single_decl: TYPE IDENTIFIER ASSIGN expr ';'  */
#line 251 "parser.y"
                                      {
        // צורה: type int:x = 5;
        char* full = (yyvsp[-3].str);
        char* colon = strchr(full, ':');
        if (!colon) {
            semantic_error("Invalid format in variable declaration, expected format: type int:x", (yylsp[-3]).first_line);
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
            semantic_error("Variable already declared in this scope", (yylsp[-3]).first_line);
            YYABORT;
        }

        Node* var_node = create_node(varname, 0);
        (yyval.node) = create_node("=", 2, var_node, (yyvsp[-1].node));
    }
#line 1773 "parser.tab.c"
    break;

  case 45: /* var_single_decl: TYPE type_name ':' IDENTIFIER ASSIGN expr ';'  */
#line 283 "parser.y"
                                                    {
        // צורה: type INT : x = 5;
        VarType t;
        if (strcmp((yyvsp[-5].node)->name, "INT") == 0) t = TYPE_INT;
        else if (strcmp((yyvsp[-5].node)->name, "REAL") == 0) t = TYPE_REAL;
        else if (strcmp((yyvsp[-5].node)->name, "CHAR") == 0) t = TYPE_CHAR;
        else if (strcmp((yyvsp[-5].node)->name, "BOOL") == 0) t = TYPE_BOOL;
        else if (strcmp((yyvsp[-5].node)->name, "INT_PTR") == 0) t = TYPE_INT_PTR;
        else if (strcmp((yyvsp[-5].node)->name, "REAL_PTR") == 0) t = TYPE_REAL_PTR;
        else if (strcmp((yyvsp[-5].node)->name, "CHAR_PTR") == 0) t = TYPE_CHAR_PTR;
        else t = TYPE_VOID;

        if (!insert_symbol((yyvsp[-3].str), VAR_SYM, t, -1)) {
            semantic_error("Variable already declared in this scope", (yylsp[-3]).first_line);
            YYABORT;
        }

        Node* var_node = create_node((yyvsp[-3].str), 0);
        (yyval.node) = create_node("=", 2, var_node, (yyvsp[-1].node));
    }
#line 1798 "parser.tab.c"
    break;

  case 46: /* args: expr  */
#line 306 "parser.y"
           { (yyval.node) = create_node("ARGS", 1, (yyvsp[0].node)); }
#line 1804 "parser.tab.c"
    break;

  case 47: /* args: expr ',' args  */
#line 307 "parser.y"
                    { (yyval.node) = create_node("ARGS", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1810 "parser.tab.c"
    break;

  case 48: /* args: %empty  */
#line 308 "parser.y"
                  { (yyval.node) = create_node("ARGS NONE", 0); }
#line 1816 "parser.tab.c"
    break;

  case 49: /* expr: expr ASSIGN expr  */
#line 312 "parser.y"
                          { (yyval.node) = create_node("=", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1822 "parser.tab.c"
    break;

  case 50: /* expr: expr ADD expr  */
#line 313 "parser.y"
                          { (yyval.node) = create_node("+", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1828 "parser.tab.c"
    break;

  case 51: /* expr: expr SUB expr  */
#line 314 "parser.y"
                          { (yyval.node) = create_node("-", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1834 "parser.tab.c"
    break;

  case 52: /* expr: expr MUL expr  */
#line 315 "parser.y"
                          { (yyval.node) = create_node("*", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1840 "parser.tab.c"
    break;

  case 53: /* expr: expr DIV expr  */
#line 316 "parser.y"
                          { (yyval.node) = create_node("/", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1846 "parser.tab.c"
    break;

  case 54: /* expr: SUB expr  */
#line 317 "parser.y"
                            { (yyval.node) = create_node("-", 2, create_node("0", 0), (yyvsp[0].node)); }
#line 1852 "parser.tab.c"
    break;

  case 55: /* expr: expr EQ expr  */
#line 318 "parser.y"
                          { (yyval.node) = create_node("==", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1858 "parser.tab.c"
    break;

  case 56: /* expr: expr NEQ expr  */
#line 319 "parser.y"
                          { (yyval.node) = create_node("!=", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1864 "parser.tab.c"
    break;

  case 57: /* expr: expr LEQ expr  */
#line 320 "parser.y"
                          { (yyval.node) = create_node("<=", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1870 "parser.tab.c"
    break;

  case 58: /* expr: expr GEQ expr  */
#line 321 "parser.y"
                          { (yyval.node) = create_node(">=", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1876 "parser.tab.c"
    break;

  case 59: /* expr: expr LT expr  */
#line 322 "parser.y"
                          { (yyval.node) = create_node("<", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1882 "parser.tab.c"
    break;

  case 60: /* expr: expr GT expr  */
#line 323 "parser.y"
                          { (yyval.node) = create_node(">", 2, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1888 "parser.tab.c"
    break;

  case 61: /* expr: '(' expr ')'  */
#line 324 "parser.y"
                          { (yyval.node) = (yyvsp[-1].node); }
#line 1894 "parser.tab.c"
    break;

  case 62: /* expr: IDENTIFIER  */
#line 325 "parser.y"
                          { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 1900 "parser.tab.c"
    break;

  case 63: /* expr: INT_LITERAL  */
#line 326 "parser.y"
                          { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 1906 "parser.tab.c"
    break;

  case 64: /* expr: REAL_LITERAL  */
#line 327 "parser.y"
                          { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 1912 "parser.tab.c"
    break;

  case 65: /* expr: CHAR_LITERAL  */
#line 328 "parser.y"
                          { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 1918 "parser.tab.c"
    break;

  case 66: /* expr: STRING_LITERAL  */
#line 329 "parser.y"
                          { (yyval.node) = create_node((yyvsp[0].str), 0); }
#line 1924 "parser.tab.c"
    break;

  case 67: /* expr: TRUE  */
#line 330 "parser.y"
                          { (yyval.node) = create_node("TRUE", 0); }
#line 1930 "parser.tab.c"
    break;

  case 68: /* expr: FALSE  */
#line 331 "parser.y"
                          { (yyval.node) = create_node("FALSE", 0); }
#line 1936 "parser.tab.c"
    break;


#line 1940 "parser.tab.c"

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
  *++yylsp = yyloc;

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

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 334 "parser.y"



int main() {
    if (yyparse() == 0 && root != NULL) {
        print_ast(root, 0);
    }
    return 0;
}



