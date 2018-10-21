//
// Created by unixeno on 2018/9/19.
// The Lexer for c-- language
//

#ifndef C_LEX_H
#define C_LEX_H

#include <stdint.h>

#define LEX_TOKEN_LENGTH 128

typedef enum
{
    TOKEN_IDENTIFIER,   // normal identifier
    TOKEN_KEYWORD,   // identifier
    TOKEN_END,          // end of file
    TOKEN_CHAR,         // character
    TOKEN_INTEGER,      // a integer number
    TOKEN_FLOAT,        // float number, always be double
    TOKEN_STRING,       // string
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_MUL,          // *
    TOKEN_DIV,          // /
    TOKEN_EQUAL,        // =
    TOKEN_COMMA,        // ,
    TOKEN_DOT,          // .
    TOKEN_COLON,        // :
    TOKEN_SEMICOLON,    // ;
    TOKEN_MOD,          // %
    TOKEN_LBRACKET,     // (
    TOKEN_RBRACKET,     // )
    TOKEN_AND,          // &
    TOKEN_HASH,         // #
    TOKEN_NOT,          // !
    TOKEN_XOR,          // ^
    TOKEN_TILDE,        // ~
    TOKEN_LCURLY_BRACKETS, // {
    TOKEN_RCURLY_BRACKETS, // }
    TOKEN_LSCUARE_BRACKETS,// [
    TOKEN_RSCUARE_BRACKETS,// ]
    TOKEN_VERTICAL_BAR, // |
    TOKEN_LESS_THAN,    // <
    TOKEN_GREATER_THAN, // >
    TOKEN_QUESTION,     // ?

    TOKEN_PLUSPLUS,     // ++
    TOKEN_MINUSMINUS,   // --
    TOKEN_PLUSEQUAL,    // +=
    TOKEN_MINUSEQUAL,   // -=
    TOKEN_MULEQUAL,     // *=
    TOKEN_DIVEQUAL,     // /=
    TOKEN_DOUBLEEQUAL,  // ==
    TOKEN_NOTEQUAL,     // !=
    TOKEN_LEQUAL,       // <=
    TOKEN_GEQUAL,       // >=
    TOKEN_LOGICAL_AND,  // &&
    TOKEN_LOGICAL_OR,   // ||
    TOKEN_SHIFT_LEFT,   // <<
    TOKEN_SHIFT_RIGHT,  // >>
    TOKEN_AND_EQUAL,    // &=
    TOKEN_OR_EQUAL,     // |=
    TOKEN_XOR_EQUAL,    // ^=

    TOKEN_SIZEOF,       // sizeof

}TokenType;

typedef struct
{
    union {
        uint32_t value_uint32;
        int32_t  value_int32;
        uint64_t value_uint64;
        int64_t  value_int64;
    }value;
    enum {
        SUFFIX_INT_NONE,
        SUFFIX_INT_L,
        SUFFIX_INT_LL,
        SUFFIX_INT_U,
        SUFFIX_INT_UL,
        SUFFIX_INT_ULL
    }suffix;
}Integer;

typedef struct
{
    union {
        float  value_float;
        double value_double;
    }value;
    enum {
        SUFFIX_FLT_NONE,
        SUFFIX_FLT_F
    }suffix;
}Float;

typedef struct
{
    TokenType token_type;
    wchar_t token_string[LEX_TOKEN_LENGTH];
    char *filename;
    uint32_t line;
    uint32_t true_line;
    uint32_t column;
    union {
        Integer integer;
        Float   floats;
    }data;
}Token;

int lex_init(char *source_filename);
int lex_gettoken(Token *token);
const char *lex_get_token_string(TokenType type);
#endif //C_LEX_H
