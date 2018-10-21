//
// Created by unixeno on 2018/9/19.
//

#include <stdint.h>
#include <ctype.h>
#include "lex.h"
#include "io/io_handler.h"
#include "io/error_handler.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    STATE_INIT,
    STATE_FINAL,
    STATE_CHAR,
    STATE_STRING,
    STATE_STRING_NOT_END,
    STATE_ESCAPE_CHAR,
    STATE_NUMBER,
    STATE_NUMBER_SUFFIX,
    STATE_NUMBER_SPECIAL,
    STATE_NUMBER_HEX,
    STATE_NUMBER_BIN,
    STATE_NUMBER_OCT,
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

bool _iswoctdigit(wchar_t ch)
{
    if (wcschr(L"01234567", ch))
        return true;
    return false;
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
                }
                else if (ch.ch == WEOF)                 // meet the end of this file
                {
                    tmp_token.token_type = TOKEN_END;
                    wcscpy(tmp_token.token_string, L"END_OF_FILE_TOKEN");
                    lex_state = STATE_FINAL;
                }
                else if (ch.ch == L'0')
                {
                    lex_state = STATE_NUMBER_SPECIAL;
                }
                else if (iswdigit(ch.ch))
                {
                    lex_state = STATE_NUMBER;
                }
                else if (iswalpha(ch.ch) || ch.ch == '_')
                {
                    lex_state = STATE_IDENTIFIER;
                    _lex_parser_identifier();
                    lex_state = STATE_FINAL;
                }
                else if (ch.ch == '/')
                {
                    lex_state = STATE_MAYBE_COMMENT;        // maybe a comment?
                }
                else if (ch.ch == '\'')
                {
                    lex_state = STATE_CHAR;
                }
                else if (ch.ch == '"')
                {
                    lex_state = STATE_STRING;
                }
                else
                {
                    lex_state = STATE_FINAL;
                    switch (ch.ch)
                    {
                        case ',':
                            tmp_token.token_type = TOKEN_COMMA;
                            break;
                        case '.':
                            tmp_token.token_type = TOKEN_DOT;
                            break;
                        case ':':
                            tmp_token.token_type = TOKEN_COLON;
                            break;
                        case ';':
                            tmp_token.token_type = TOKEN_SEMICOLON;
                            break;
                        case '(':
                            tmp_token.token_type = TOKEN_LBRACKET;
                            break;
                        case ')':
                            tmp_token.token_type = TOKEN_RBRACKET;
                            break;
                        case '{':
                            tmp_token.token_type = TOKEN_LCURLY_BRACKETS;
                            break;
                        case '}':
                            tmp_token.token_type = TOKEN_RCURLY_BRACKETS;
                            break;
                        case '[':
                            tmp_token.token_type = TOKEN_LSCUARE_BRACKETS;
                            break;
                        case ']':
                            tmp_token.token_type = TOKEN_RSCUARE_BRACKETS;
                            break;
                        case '%':
                            tmp_token.token_type = TOKEN_MOD;
                            break;
                        case '?':
                            tmp_token.token_type = TOKEN_QUESTION;
                            break;
                        case '#':
                            tmp_token.token_type = TOKEN_HASH;
                            break;
                        case '~':
                            tmp_token.token_type = TOKEN_TILDE;
                            break;
                        case '^':
                            io_handler_getchar(&ch);
                            if (ch.ch == '=')
                                tmp_token.token_type = TOKEN_XOR_EQUAL;
                            else
                            {
                                io_handler_ungetchar();
                                tmp_token.token_type = TOKEN_XOR;
                            }
                            break;
                        case '+':
                            io_handler_getchar(&ch);
                            if (ch.ch == '+')
                                tmp_token.token_type = TOKEN_PLUSPLUS;
                            else if (ch.ch == '=')
                                tmp_token.token_type = TOKEN_PLUSEQUAL;
                            else
                            {
                                io_handler_ungetchar();
                                tmp_token.token_type = TOKEN_PLUS;
                            }
                            break;
                        case '-':
                            io_handler_getchar(&ch);
                            if (ch.ch == '-')
                                tmp_token.token_type = TOKEN_MINUSMINUS;
                            else if (ch.ch == '=')
                                tmp_token.token_type = TOKEN_MINUSEQUAL;
                            else
                            {
                                io_handler_ungetchar();
                                tmp_token.token_type = TOKEN_MINUS;
                            }
                            break;
                        case '=':
                            io_handler_getchar(&ch);
                            if (ch.ch == '=')
                                tmp_token.token_type = TOKEN_DOUBLEEQUAL;
                            else
                            {
                                io_handler_ungetchar();
                                tmp_token.token_type = TOKEN_EQUAL;
                            }
                            break;
                        case '>':
                            io_handler_getchar(&ch);
                            if (ch.ch == '=')
                                tmp_token.token_type = TOKEN_GEQUAL;
                            else if (ch.ch == '>')
                                tmp_token.token_type = TOKEN_SHIFT_RIGHT;
                            else
                            {
                                io_handler_ungetchar();
                                tmp_token.token_type = TOKEN_GREATER_THAN;
                            }
                            break;
                        case '<':
                            io_handler_getchar(&ch);
                            if (ch.ch == '=')
                                tmp_token.token_type = TOKEN_LEQUAL;
                            else if (ch.ch == '<')
                                tmp_token.token_type = TOKEN_SHIFT_LEFT;
                            else
                            {
                                io_handler_ungetchar();
                                tmp_token.token_type = TOKEN_LESS_THAN;
                            }
                            break;
                        case '!':
                            io_handler_getchar(&ch);
                            if (ch.ch == '=')
                                tmp_token.token_type = TOKEN_NOTEQUAL;
                            else
                            {
                                io_handler_ungetchar();
                                tmp_token.token_type = TOKEN_NOT;
                            }
                            break;
                        case '*':
                            io_handler_getchar(&ch);
                            if (ch.ch == '=')
                                tmp_token.token_type = TOKEN_MULEQUAL;
                            else
                            {
                                io_handler_ungetchar();
                                tmp_token.token_type = TOKEN_MUL;
                            }
                            break;
                        case '&':
                            io_handler_getchar(&ch);
                            if (ch.ch == '&')
                                tmp_token.token_type = TOKEN_LOGICAL_AND;
                            else if (ch.ch == '=')
                                tmp_token.token_type = TOKEN_AND_EQUAL;
                            else
                            {
                                io_handler_ungetchar();
                                tmp_token.token_type = TOKEN_AND;
                            }
                            break;
                        case '|':
                            io_handler_getchar(&ch);
                            if (ch.ch == '|')
                                tmp_token.token_type = TOKEN_LOGICAL_OR;
                            else if (ch.ch == '=')
                                tmp_token.token_type = TOKEN_OR_EQUAL;
                            else
                            {
                                io_handler_ungetchar();
                                tmp_token.token_type = TOKEN_VERTICAL_BAR;
                            }
                            break;
                        default:
                            io_handler_gettoken(tmp_token.token_string, LEX_TOKEN_LENGTH);
                            error_print(tmp_token, "stray '%ls' in program", tmp_token.token_string);
                    }// end of switch
                    io_handler_gettoken(tmp_token.token_string, LEX_TOKEN_LENGTH);
                }
                break;
            }   // end STATE_INIT state
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
                else if (ch.ch == '=')
                {
                    lex_state = STATE_FINAL;
                    tmp_token.token_type = TOKEN_DIVEQUAL;
                    io_handler_gettoken(tmp_token.token_string, LEX_TOKEN_LENGTH);
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
            case STATE_NUMBER:
            {
                if (iswdigit(ch.ch))
                {
                    continue;
                }
                else if (ch.ch == L'.')
                {
                    lex_state = STATE_FLOAT;
                }
                else if (iswalpha(ch.ch))
                {
                    lex_state = STATE_NUMBER_SUFFIX;
                    io_handler_ungetchar();                 // put back the suffix, cause we parse all
                                                            //   suffix in suffix state.
                    io_handler_gettoken(tmp_token.token_string, LEX_TOKEN_LENGTH);
                }
                else
                {
                    io_handler_ungetchar();                 // current char is not a part of this number
                                                            //   so we should put it back.
                    tmp_token.token_type = TOKEN_INTEGER;
                    tmp_token.data.integer.suffix = SUFFIX_INT_NONE;
                    io_handler_gettoken(tmp_token.token_string, LEX_TOKEN_LENGTH);
                    lex_state = STATE_FINAL;
                }
                break;
            }
            case STATE_NUMBER_SUFFIX:
            {
                if (iswalpha(ch.ch))
                {
                    continue;
                }
                else
                {
                    io_handler_ungetchar();

                    wchar_t suffix[LEX_TOKEN_LENGTH];
                    io_handler_gettoken(suffix, LEX_TOKEN_LENGTH);
                    if (wcscmp(suffix, L"l") == 0|| wcscmp(suffix, L"L") == 0)
                    {
                        tmp_token.data.integer.suffix = SUFFIX_INT_L;
                    }
                    else if (wcscmp(suffix, L"ll") == 0 || wcscmp(suffix, L"LL") == 0)
                    {
                        tmp_token.data.integer.suffix = SUFFIX_INT_L;
                    }
                    else if (wcscmp(suffix, L"u") == 0 || wcscmp(suffix, L"U") == 0)
                    {
                        tmp_token.data.integer.suffix = SUFFIX_INT_U;
                    }
                    else if (wcscmp(suffix, L"ul") == 0 || wcscmp(suffix, L"UL") == 0)
                    {
                        tmp_token.data.integer.suffix = SUFFIX_INT_UL;
                    }
                    else if (wcscmp(suffix, L"ull") == 0 || wcscmp(suffix, L"ULL") == 0)
                    {
                        tmp_token.data.integer.suffix = SUFFIX_INT_ULL;
                    }
                    else
                    {
                        error_print(tmp_token, "invalid suffix \"%ls\" on floating constant\n    %ls\n", suffix, tmp_token.token_string);
                    }
                }
                lex_state = STATE_FINAL;

            }
            case STATE_FLOAT:
            {
                if (iswdigit(ch.ch))
                {
                    continue;
                }
                else
                {
                    io_handler_ungetchar();
                    io_handler_gettoken(tmp_token.token_string, LEX_TOKEN_LENGTH);
                    tmp_token.token_type = TOKEN_FLOAT;
                    lex_state = STATE_FINAL;
                }
                break;
            }
            case STATE_CHAR:
            {
                if (ch.ch == '\\')
                {
                    lex_state = STATE_ESCAPE_CHAR;
                }
                else if (ch.ch == '\'')
                {
                    lex_state = STATE_FINAL;
                    tmp_token.token_type = TOKEN_CHAR;
                    io_handler_gettoken(tmp_token.token_string, LEX_TOKEN_LENGTH);
                }
                break;
            }
            case STATE_STRING:
            {
                if (ch.ch == L'\\')
                {
                    lex_state = STATE_STRING_NOT_END;
                }
                else if(ch.ch == '"')       // end of string
                {
                    io_handler_gettoken(tmp_token.token_string, LEX_TOKEN_LENGTH);
                    tmp_token.token_type = TOKEN_STRING;
                    lex_state = STATE_FINAL;
                }
                else if (ch.ch == '\n')     // invalid char
                {
                    io_handler_gettoken(tmp_token.token_string, LEX_TOKEN_LENGTH);
                    error_print(tmp_token, "missing '\"' in string %ls", tmp_token.token_string);
                }
                break;
            }
            case STATE_STRING_NOT_END:
            {
                lex_state = STATE_STRING;       // just pass the next char
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
        "TOKEN_KEYWORD",   // identifier
        "TOKEN_END",          // end of file
        "TOKEN_CHAR",         // character
        "TOKEN_INTEGER",      // a integer number
        "TOKEN_FLOAT",        // float number, always be double
        "TOKEN_STRING",       // string
        "TOKEN_PLUS",         // +
        "TOKEN_MINUS",        // -
        "TOKEN_MUL",          // *
        "TOKEN_DIV",          // /
        "TOKEN_EQUAL",        // =
        "TOKEN_COMMA",        // ,
        "TOKEN_DOT",          // .
        "TOKEN_COLON",        // :
        "TOKEN_SEMICOLON",    // ;
        "TOKEN_MOD",          // %
        "TOKEN_LBRACKET",     // (
        "TOKEN_RBRACKET",     // )
        "TOKEN_AND",          // &
        "TOKEN_HASH",         // #
        "TOKEN_NOT",          // !
        "TOKEN_XOR",          // ^
        "TOKEN_TILDE",        // ~
        "TOKEN_LCURLY_BRACKETS", // {
        "TOKEN_RCURLY_BRACKETS", // }
        "TOKEN_LSCUARE_BRACKETS",// [
        "TOKEN_RSCUARE_BRACKETS",// ]
        "TOKEN_VERTICAL_BAR", // |
        "TOKEN_LESS_THAN",    // <
        "TOKEN_GREATER_THAN", // >
        "TOKEN_QUESTION",     // ?

        "TOKEN_PLUSPLUS",     // ++
        "TOKEN_MINUSMINUS",   // --
        "TOKEN_PLUSEQUAL",    // +=
        "TOKEN_MINUSEQUAL",   // -=
        "TOKEN_MULEQUAL",     // *=
        "TOKEN_DIVEQUAL",     // /=
        "TOKEN_DOUBLEEQUAL",  // ==
        "TOKEN_NOTEQUAL",     // !=
        "TOKEN_LEQUAL",       // <=
        "TOKEN_GEQUAL",       // >=
        "TOKEN_LOGICAL_AND",  // &&
        "TOKEN_LOGICAL_OR",   // ||
        "TOKEN_SHIFT_LEFT",   // <<
        "TOKEN_SHIFT_RIGHT",  // >>
        "TOKEN_AND_EQUAL",    // &=
        "TOKEN_OR_EQUAL",     // |=
        "TOKEN_XOR_EQUAL",    // ^=
};

const char *lex_get_token_string(TokenType type)
{
    return TokenTypeString[type];
}
