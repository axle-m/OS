void parse(char **lexed, parsed_command_t *out_cmd) {
    if(lexed[0] == NULL){
        out_cmd->name = NULL;
        out_cmd->flag_count = 0;
        out_cmd->arg_count = 0;
        return;
    }

    out_cmd->name = lexed[0];
    out_cmd->flag_count = 0;
    out_cmd->arg_count = 0;

    for(int i = 1; lexed[i] != NULL; i++){
        if(*lexed[i] == '-'){
            out_cmd->flags[out_cmd->flag_count++] = lexed[i];
        }
        else{
            out_cmd->args[out_cmd->arg_count++] = lexed[i];
        }
    }
}