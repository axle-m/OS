#pragma once

typedef struct {
    char *name;
    char *flags[8];
    int flag_count;
    char *args[16];
    int arg_count;
} parsed_command_t;

// Looks up the command in the dictionary and executes it
void execute_command(parsed_command_t *cmd);