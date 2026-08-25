#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "expand.h"

static void expand_text(char *text)
{
    char result[MAX_TOKEN_LEN];
    int i = 0;
    int j = 0;

    while (text[i] != '\0' && j < MAX_TOKEN_LEN - 1) {
        if (text[i] == '$') {
            char name[128];
            int n = 0;

            i++;

            if (text[i] == '{') {
                i++;

                while (text[i] != '\0' &&
                       text[i] != '}' &&
                       n < (int)sizeof(name) - 1) {
                    name[n++] = text[i++];
                }

                if (text[i] == '}') {
                    i++;
                }
            } else {
                while ((isalnum((unsigned char)text[i]) || text[i] == '_') &&
                       n < (int)sizeof(name) - 1) {
                    name[n++] = text[i++];
                }
            }

            name[n] = '\0';

            if (n == 0) {
                result[j++] = '$';
                continue;
            }

            const char *value = getenv(name);

            if (value != NULL) {
                while (*value != '\0' && j < MAX_TOKEN_LEN - 1) {
                    result[j++] = *value++;
                }
            }
        } else {
            result[j++] = text[i++];
        }
    }

    result[j] = '\0';
    strncpy(text, result, MAX_TOKEN_LEN - 1);
    text[MAX_TOKEN_LEN - 1] = '\0';
}

void expand_variables(pipeline_t *pipeline)
{
    for (int i = 0; i < pipeline->command_count; i++) {
        command_t *cmd = &pipeline->commands[i];

        for (int j = 0; j < cmd->argc; j++) {
            expand_text(cmd->argv[j]);
        }

        if (cmd->input[0] != '\0') {
            expand_text(cmd->input);
        }

        if (cmd->output[0] != '\0') {
            expand_text(cmd->output);
        }
    }
}
