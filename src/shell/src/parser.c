#include "../include/parser.h"
#include <stddef.h>
#include "parser.h"

extern void cmd_help(parsed_command_t *cmd);
extern void cmd_clear(parsed_command_t *cmd);

static const command_t dictionary[] = {
    {
        .name = "help",
        .description = "List all available commands and their descriptions",
        .execute = cmd_help
    },
    {
        .name = "clear",
        .description = "Clear the console input",
        .execute = cmd_clear
    },
    {
        .name = "echo",
        .description = "Print out the command arguments",
        .execute = cmd_echo
    },
};


#define COMMAND_COUNT (sizeof(dictionary) / sizeof(dictionary[0]))



void parse(char **lexed, parsed_command_t *out_cmd)
{
    if (lexed[0] == NULL)
    {
        out_cmd->name = NULL;
        out_cmd->flag_count = 0;
        out_cmd->arg_count = 0;
        return;
    }

    out_cmd->name = lexed[0];
    out_cmd->flag_count = 0;
    out_cmd->arg_count = 0;

    for (int i = 1; lexed[i] != NULL; i++)
    {
        if (*lexed[i] == '-')
        {
            out_cmd->flags[out_cmd->flag_count++] = lexed[i];
        }
        else
        {
            out_cmd->args[out_cmd->arg_count++] = lexed[i];
        }
    }
}

void execute_command(parsed_command_t *cmd){
    if(cmd->name == NULL) return;

    for(int i = 0; i < COMMAND_COUNT; i++){
        if(kstrcmp(cmd->name, dictionary[i].name) == 0){
            dictionary[i].execute(cmd);
            return;
        }
    }

    print("Command \"");
    print(cmd->name);
    print("\" not found.\n");
}