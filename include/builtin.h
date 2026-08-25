#ifndef BUILTIN_H
#define BUILTIN_H

#include "parser.h"

#define BUILTIN_NONE 0
#define BUILTIN_DONE 1
#define BUILTIN_EXIT 2

int builtin_execute(command_t *cmd);

#endif
