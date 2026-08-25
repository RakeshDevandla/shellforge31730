#include <ctype.h>
#include <stdio.h>
#include "lexer.h"

int lexer(const char *input, token_list_t *list)
{
    int i = 0;

    token_list_init(list);

    while (input[i] != '\0') {
        char word[MAX_TOKEN_LEN];
        int j = 0;

        while (isspace((unsigned char)input[i])) {
            i++;
        }

        if (input[i] == '\0') {
            break;
        }

        if (input[i] == '|') {
            token_add(list, TOKEN_PIPE, "|");
            i++;
            continue;
        }

        if (input[i] == '<') {
            token_add(list, TOKEN_INPUT, "<");
            i++;
            continue;
        }

        if (input[i] == '>') {
            if (input[i + 1] == '>') {
                token_add(list, TOKEN_APPEND, ">>");
                i += 2;
            } else {
                token_add(list, TOKEN_OUTPUT, ">");
                i++;
            }
            continue;
        }

        if (input[i] == '&') {
            token_add(list, TOKEN_BACKGROUND, "&");
            i++;
            continue;
        }

        while (input[i] != '\0' &&
               !isspace((unsigned char)input[i]) &&
               input[i] != '|' &&
               input[i] != '<' &&
               input[i] != '>' &&
               input[i] != '&') {

            if (input[i] == '\'' || input[i] == '"') {
                char quote = input[i++];
                while (input[i] != '\0' && input[i] != quote) {
                    if (j < MAX_TOKEN_LEN - 1) {
                        word[j++] = input[i];
                    }
                    i++;
                }

                if (input[i] != quote) {
                    fprintf(stderr, "Lexer error: unterminated quote\n");
                    return -1;
                }

                i++;
            } else if (input[i] == '\\') {
                i++;

                if (input[i] != '\0' && j < MAX_TOKEN_LEN - 1) {
                    word[j++] = input[i++];
                }
            } else {
                if (j < MAX_TOKEN_LEN - 1) {
                    word[j++] = input[i];
                }
                i++;
            }
        }

        word[j] = '\0';

        if (j > 0) {
            token_add(list, TOKEN_WORD, word);
        }
    }

    token_add(list, TOKEN_END, "END");
    return 0;
}
