#include <stdio.h>
#include <string.h>
#include "parser.h"

void command_init(command_t *cmd)
{
    cmd->argc = 0;
    cmd->input[0] = '\0';
    cmd->output[0] = '\0';
    cmd->append = 0;
    cmd->background = 0;

    for (int i = 0; i < MAX_ARGS; i++) {
        cmd->argv[i] = NULL;
    }
}

int parse(const token_list_t *tokens, pipeline_t *pipeline)
{
    int current = 0;

    pipeline->command_count = 1;
    command_init(&pipeline->commands[0]);

    for (int i = 0; i < tokens->count; i++) {
        const token_t *token = &tokens->tokens[i];
        command_t *cmd = &pipeline->commands[current];

        if (token->type == TOKEN_WORD) {
            if (cmd->argc >= MAX_ARGS - 1) {
                fprintf(stderr, "Parser error: too many arguments\n");
                return 0;
            }

            cmd->argv[cmd->argc++] = (char *)token->text;
        }
        else if (token->type == TOKEN_INPUT ||
                 token->type == TOKEN_OUTPUT ||
                 token->type == TOKEN_APPEND) {
            if (i + 1 >= tokens->count ||
                tokens->tokens[i + 1].type != TOKEN_WORD) {
                fprintf(stderr, "Parser error: filename expected\n");
                return 0;
            }

            const char *filename = tokens->tokens[i + 1].text;

            if (token->type == TOKEN_INPUT) {
                strncpy(cmd->input, filename, MAX_TOKEN_LEN - 1);
                cmd->input[MAX_TOKEN_LEN - 1] = '\0';
            } else {
                strncpy(cmd->output, filename, MAX_TOKEN_LEN - 1);
                cmd->output[MAX_TOKEN_LEN - 1] = '\0';
                cmd->append = (token->type == TOKEN_APPEND);
            }

            i++;
        }
        else if (token->type == TOKEN_BACKGROUND) {
            cmd->background = 1;
        }
        else if (token->type == TOKEN_PIPE) {
            cmd->argv[cmd->argc] = NULL;

            if (current >= MAX_COMMANDS - 1) {
                fprintf(stderr, "Parser error: too many commands in pipeline\n");
                return 0;
            }

            current++;
            pipeline->command_count++;
            command_init(&pipeline->commands[current]);
        }
        else if (token->type == TOKEN_END) {
            break;
        }
    }

    pipeline->commands[current].argv[pipeline->commands[current].argc] = NULL;
    return 1;
}

void pipeline_print(const pipeline_t *pipeline)
{
    printf("\n========== PIPELINE ==========\n");

    for (int i = 0; i < pipeline->command_count; i++) {
        const command_t *cmd = &pipeline->commands[i];

        printf("\nCommand %d\n", i + 1);
        printf("------------------------------\n");
        printf("Arguments\n");

        for (int j = 0; j < cmd->argc; j++) {
            printf("argv[%d] = %s\n", j, cmd->argv[j]);
        }

        printf("Input      : %s\n", cmd->input[0] ? cmd->input : "None");
        printf("Output     : %s\n", cmd->output[0] ? cmd->output : "None");
        printf("Append     : %s\n", cmd->append ? "Yes" : "No");
        printf("Background : %s\n", cmd->background ? "Yes" : "No");
    }

    printf("==============================\n");
}
