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
    TOKEN_END,          // end of file
    TOKEN_INTEGER,      // a integer number
    TOKEN_FLOAT,        // float number, always be double
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_MUL,          // *
    TOKEN_DIV,          // /
    TOKEN_EQUAL,        // =
    TOKEN_COMMA,        // ,
    TOKEN_DOT,          // .
    TOKEN_COLON,        // :
    TOKEN_SEMICOLON,    // ;
    TOKEN_PERSENT,      // %
    TOKEN_DOLLAR,       // $
    TOKEN_BACKSLASH,    // '\'
    TOKEN_LBRACKET,     // (
    TOKEN_RBRACKET,     // )
    TOKEN_AMPERSAND,    // &
    TOKEN_UNDERSCORE,   // _
    TOKEN_AT,           // @
    TOKEN_HASH,         // #
    TOKEN_EXCLAMATION,  // !
    TOKEN_CAREC,        // ^
    TOKEN_TILDE,        // ~
    TOKEN_BACKTICK,     // `
    TOKEN_APOSTROPHE,   // '
    TOKEN_QUOTE,        // "
    TOKEN_LCURLY_BRACKETS, // {
    TOKEN_RCURLY_BRACKETS, // }
    TOKEN_LSCUARE_BRACKETS,// [
    TOKEN_RSCUARE_BRACKETS,// ]
    TOKEN_VERTICAL_BAR, // |
    TOKEN_LESS_THAN,    // <
    TOKEN_GREATER_THAN, // >
    TOKEN_QUESTION,     // ?
}TokenType;

typedef struct
{
    TokenType token_type;
    wchar_t token_string[LEX_TOKEN_LENGTH];
    char *filename;
    uint32_t line;
    uint32_t true_line;
    uint32_t column;
}Token;

int lex_init(char *source_filename);
int lex_gettoken(Token *token);
char *lex_get_token_string(TokenType type);
#endif //C_LEX_H
