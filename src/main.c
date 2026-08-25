#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "lexer.h"
#include "parser.h"
#include "expand.h"

int main(void)
{
    printf("=====================================\n");
    printf("Shellforge\n");
    printf("A Unix Style Shell written in C\n");
    printf("=====================================\n");

    while (1) {
        char *line = readline("shellforge$ ");

        if (line == NULL) {
            printf("\nGoodbye!\n");
            break;
        }

        if (strlen(line) == 0) {
            free(line);
            continue;
        }

        add_history(line);

        token_list_t tokens;
        pipeline_t pipeline;

        if (lexer(line, &tokens) == 0) {
            token_print(&tokens);

            if (parse(&tokens, &pipeline)) {
                expand_variables(&pipeline);
                pipeline_print(&pipeline);
            }
        }

        if (strcmp(line, "exit") == 0) {
            free(line);
            printf("Exiting...\n");
            break;
        }

        free(line);
    }

    return 0;
}
