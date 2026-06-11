#pragma once

#include "../../libk/include/ktypes.h"

#define MAX_ARGS 16

typedef struct {
    const char *flag;
    const char *description;
} flag_t;

typedef struct
{
    char *name;
    char *flags[MAX_ARGS];
    int flag_count;
    char *args[MAX_ARGS];
    int arg_count;
} parsed_command_t;
 
typedef void (*command_func)(parsed_command_t *);

typedef struct {
    const char *name;
    const char *description;
    command_func execute;

    flag_t flags[4];
    int flag_count;
} command_t;

extern const command_t dictionary[];

void parse(char **lexed, parsed_command_t *out_cmd);

void execute_command(parsed_command_t *cmd);