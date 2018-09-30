//
// Created by unixeno on 2018/9/19.
//

#include <stdint.h>
#include <ctype.h>
#include "lex.h"
#include "../io/io_handler.h"
#include <stdbool.h>
#include <string.h>

typedef enum
{
    STATE_INIT,
    STATE_NUMBER,
    STATE_IDENTIFIER,
    STATE_S_COMMENT,
}LexState;

int lex_init(char *source_filename)
{
    io_handler_init();
    io_handler_open(source_filename);
}

static Token tmp_token;
static LexState lex_state;

bool _iswhexdigit(wchar_t ch)
{
    if (iswdigit(ch) || wcschr(L"abcdefABCDEF", ch))
        return true;
    else
        return false;
}

void _lex_parser_number_hex()
{
    CharStruct ch;
    while (1)
    {
        io_handler_getchar(&ch);
        if (!_iswhexdigit(ch.ch))
        {
            break;
        }
    }
}

void _lex_parser_number()
{
    CharStruct ch;
    while (1)
    {
        io_handler_getchar(&ch);
        if (iswdigit(ch.ch))
        {
            continue;
        }
    }

}

void _lex_parser_identifier()
{
    CharStruct ch;
    while (1)
    {
        io_handler_getchar(&ch);
        if (iswalpha(ch.ch) || ch.ch == '_')
            continue;
        io_handler_ungetchar();
        break;
    }
    io_handler_gettoken(tmp_token.token_string, LEX_TOKEN_LENGTH);
    tmp_token.token_type = TOKEN_IDENTIFIER;
}

int lex_gettoken(Token *token)
{
    CharStruct ch;
    lex_state = STATE_INIT;
    while (1)
    {
        io_handler_getchar(&ch);
        if (lex_state == STATE_INIT)
        {
            tmp_token.filename = ch.filename;
            tmp_token.column = ch.column;
            tmp_token.line = ch.line;
            if (ch.ch == WEOF)
            {
                break;
            }
            else if (iswdigit(ch.ch))
            {
                lex_state = STATE_NUMBER;
            }
            else if (iswspace(ch.ch))
            {
                io_handler_reset();
                continue;
            }
            else if (iswalpha(ch.ch) || ch.ch == '_')
            {
                lex_state = STATE_IDENTIFIER;
                _lex_parser_identifier();
                break;
            }
        }
    }
    memcpy(token, &tmp_token, sizeof(Token));
}

static char *TokenTypeString[] = {
        "TOKEN_IDENTIFIER",   // normal identifier
        "TOKEN_INTEGER",      // a integer number
        "TOKEN_FLOAT",        // float number, always be double
        "TOKEN_PLUS",         // +
        "TOKEN_MINUS",        // -
        "TOKEN_MUL",          // *
        "TOKEN_DIV",          // /
        "TOKEN_EQUAL",        // =
        "TOKEN_COMMA",        // ,
        "TOKEN_DOT",          // .
        "TOKEN_COLON",        // :
        "TOKEN_SEMICOLON",    // ;
        "TOKEN_PERSENT",      // %
        "TOKEN_DOLLAR",       // $
        "TOKEN_BACKSLASH",    // '\'
        "TOKEN_LBRACKET",     // (
        "TOKEN_RBRACKET",     // )
        "TOKEN_AMPERSAND",    // &
        "TOKEN_UNDERSCORE",   // _
        "TOKEN_AT",           // @
        "TOKEN_HASH",         // #
        "TOKEN_EXCLAMATION",  // !
        "TOKEN_CAREC",        // ^
        "TOKEN_TILDE",        // ~
        "TOKEN_BACKTICK",     // `
        "TOKEN_APOSTROPHE",   // '
        "TOKEN_QUOTE",        // "
        "TOKEN_LCURLY_BRACKETS", // {
        "TOKEN_RCURLY_BRACKETS", // }
        "TOKEN_LSCUARE_BRACKETS",// [
        "TOKEN_RSCUARE_BRACKETS",// ]
        "TOKEN_VERTICAL_BAR", // |
        "TOKEN_LESS_THAN",    // <
        "TOKEN_GREATER_THAN", // >
        "TOKEN_QUESTION",     // ?
};

char *lex_get_token_string(TokenType type)
{
    return TokenTypeString[type];
}
