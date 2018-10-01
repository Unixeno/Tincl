//
// Created by unixeno on 2018/9/19.
//

#include <stdint.h>
#include <ctype.h>
#include "lex.h"
#include "../io/io_handler.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    STATE_INIT,
    STATE_FINAL,
    STATE_NUMBER,
    STATE_FLOAT,
    STATE_IDENTIFIER,
    STATE_MAYBE_COMMENT,
    STATE_S_COMMENT,
    STATE_M_COMMENT,
    STATE_M_COMMENT_MAYBE_END,

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
        if (iswalnum(ch.ch) || ch.ch == '_')
            continue;
        io_handler_ungetchar();
        break;
    }
    io_handler_gettoken(tmp_token.token_string, LEX_TOKEN_LENGTH);
    tmp_token.token_type = TOKEN_IDENTIFIER;
}

void _lex_parser_m_comment()
{
    CharStruct ch;
    while (1)
    {
        io_handler_getchar(&ch);
        if (lex_state == STATE_M_COMMENT)
        {
            if (ch.ch == '*')
            {
                lex_state = STATE_M_COMMENT_MAYBE_END;
            }
            continue;
        }
        else if (lex_state == STATE_M_COMMENT_MAYBE_END)
        {
            if (ch.ch == '/')
            {
                break;
            }
            else
            {
                lex_state = STATE_M_COMMENT;
            }
        }
    }
}


int lex_gettoken(Token *token)
{
    CharStruct ch;
    lex_state = STATE_INIT;
    while (1)
    {
        io_handler_getchar(&ch);
        switch (lex_state)
        {
            case STATE_INIT:
            {
                tmp_token.filename = ch.filename;
                tmp_token.column = ch.column;
                tmp_token.line = ch.line;
                if (iswspace(ch.ch) || ch.ch == '\n')    // ignore white space and line break
                {
                    io_handler_reset();
                    break;
                }
                else if (ch.ch == WEOF)                 // meet the end of this file
                {
                    tmp_token.token_type = TOKEN_END;
                    wcscpy(tmp_token.token_string, L"END_OF_FILE_TOKEN");
                    lex_state = STATE_FINAL;
                    break;
                }
                else if (iswdigit(ch.ch))
                {
                    lex_state = STATE_NUMBER;
                    break;
                }
                else if (iswalpha(ch.ch) || ch.ch == '_')
                {
                    lex_state = STATE_IDENTIFIER;
                    _lex_parser_identifier();
                    lex_state = STATE_FINAL;
                    break;
                }
                else if (ch.ch == '/')
                {
                    lex_state = STATE_MAYBE_COMMENT;        // maybe a comment?
                    break;
                }
            }
            case STATE_MAYBE_COMMENT:
            {
                if (ch.ch == '/')
                {
                    lex_state = STATE_S_COMMENT;    // yep, it's a single line comment
                }
                else if (ch.ch == '*')
                {
                    lex_state = STATE_M_COMMENT;    // it's a multi line comment
                    _lex_parser_m_comment();
                    io_handler_reset();             // the parser function won't reset the buffer
                    lex_state = STATE_INIT;         // so we should call it here
                }
                else
                {
                    lex_state = STATE_FINAL;
                    io_handler_ungetchar();         // just a div, so we should push back the next char
                    tmp_token.token_type = TOKEN_DIV;
                    io_handler_gettoken(tmp_token.token_string, LEX_TOKEN_LENGTH);
                }
                break;
            }
            case STATE_S_COMMENT:
            {
                if (ch.ch == '\n' || ch.ch == WEOF)          // end of comment
                {
                    lex_state = STATE_INIT; // back to the initial state
                    io_handler_reset();     // just ignore the comment and the line break
                }
                break;
            }
            default:
            {
                fputs("state error", stderr);
                exit(-1);
            }
        }
        if (lex_state == STATE_FINAL)
            break;
    }
    memcpy(token, &tmp_token, sizeof(Token));
}

static char *TokenTypeString[] = {
        "TOKEN_IDENTIFIER",   // normal identifier
        "TOKEN_END",
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

const char *lex_get_token_string(TokenType type)
{
    return TokenTypeString[type];
}
