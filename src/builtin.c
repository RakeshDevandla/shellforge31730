#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "builtin.h"

static int builtin_cd(command_t *cmd)
{
    const char *directory;

    if (cmd->argc == 1) {
        directory = getenv("HOME");

        if (directory == NULL) {
            fprintf(stderr, "cd: HOME is not set\n");
            return BUILTIN_DONE;
        }
    } else if (cmd->argc == 2) {
        directory = cmd->argv[1];
    } else {
        fprintf(stderr, "cd: too many arguments\n");
        return BUILTIN_DONE;
    }

    if (chdir(directory) != 0) {
        perror("cd");
    }

    return BUILTIN_DONE;
}

static int builtin_pwd(command_t *cmd)
{
    char directory[4096];

    if (cmd->argc > 1) {
        fprintf(stderr, "pwd: too many arguments\n");
        return BUILTIN_DONE;
    }

    if (getcwd(directory, sizeof(directory)) == NULL) {
        perror("pwd");
    } else {
        printf("%s\n", directory);
    }

    return BUILTIN_DONE;
}

static int builtin_echo(command_t *cmd)
{
    for (int i = 1; i < cmd->argc; i++) {
        printf("%s", cmd->argv[i]);

        if (i < cmd->argc - 1) {
            printf(" ");
        }
    }

    printf("\n");
    return BUILTIN_DONE;
}

static int builtin_exit(command_t *cmd)
{
    if (cmd->argc > 1) {
        fprintf(stderr, "exit: too many arguments\n");
        return BUILTIN_DONE;
    }

    return BUILTIN_EXIT;
}

int builtin_execute(command_t *cmd)
{
    if (cmd->argc == 0) {
        return BUILTIN_NONE;
    }

    if (strcmp(cmd->argv[0], "cd") == 0) {
        return builtin_cd(cmd);
    }

    if (strcmp(cmd->argv[0], "pwd") == 0) {
        return builtin_pwd(cmd);
    }

    if (strcmp(cmd->argv[0], "echo") == 0) {
        return builtin_echo(cmd);
    }

    if (strcmp(cmd->argv[0], "exit") == 0) {
        return builtin_exit(cmd);
    }

    return BUILTIN_NONE;
}
