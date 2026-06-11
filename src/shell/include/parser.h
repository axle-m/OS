#pragma once

#include "../../libk/include/ktypes.h"

#define MAX_ARGS 16

typedef struct
{
    char *name;
    char *flags[MAX_ARGS];
    int flag_count;
    char *args[MAX_ARGS];
    int arg_count;
} parsed_command_t;

void parse(char **lexed, parsed_command_t *out_cmd);