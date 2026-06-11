#include "../include/lexer.h"

extern void print(const char *str);

char *argv[MAX_ARGS];

char **lex(char *input)
{

    int argc = 0;
    char *token = kstrtok(input, " ");

    for (; token != NULL; token = kstrtok(NULL, " "))
    {
        if (argc >= MAX_ARGS)
        {
            print("Error: Too many arguments.\n");
            break;
        }
        argv[argc] = token;
        argc++;
    }
    argv[argc] = NULL;
    return argv;
}