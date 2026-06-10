#pragma once
#include "parser.h" // Needed to know what parsed_command_t is

// Populates the given command struct with tokens, flags, and args
void lex_command(char *raw_input, parsed_command_t *out_cmd);