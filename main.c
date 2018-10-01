#include <stdio.h>
#include <io.h>

#include "string/string.h"
#include "stack/stack.h"
#include "io/io_handler.h"
#include "lex/lex.h"
int main() {

    lex_init("test.tc");
    Token token;
    do
    {
        lex_gettoken(&token);
        printf("[%s:%d:%d][%s]=>%ls\n", token.filename, token.line, token.column,
               lex_get_token_string(token.token_type), token.token_string);
    }while (token.token_type != TOKEN_END);
    return 0;
}