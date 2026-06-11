#include "../include/runner.h"
#include "../include/lexer.h"
#include "../../libk/include/kstring.h"

extern void putchar(char c);
extern void print(const char *str);
extern void clear_screen();
extern const command_t dictionary[];
extern const int COMMAND_COUNT;

void cmd_echo(parsed_command_t *cmd) {

    if(cmd->flag_count > 0){

        for(int i = 0; i < cmd->flag_count; i++){

            if(kstrcmp(cmd->flags[i], "-n") == 0)
            {
                for(int i = 0; i < cmd->arg_count; i++){
                    print(cmd->args[i]);

                    if(i < cmd->arg_count - 1){
                        print(" ");
                    }
                }
            } 
            else if(kstrcmp(cmd->flags[i], "-lx") == 0) 
            {
                for(int j = 0; j < cmd->arg_count; j++){
                    print(cmd->args[j]);
                    print("\n");
                }
            } 
            else
            {
                print("Error - unknown flag \"");
                print(cmd->flags[i]);
                print("\"\n");
            }
        }
    } else {
        for(int i = 0; i < cmd->arg_count; i++){
            print(cmd->args[i]);

            if(i < cmd->arg_count - 1){
                print(" ");
            }
        }
        print("\n");
    }

}

void cmd_help_print(int command_id){
    //if command has flags, print Command_name [Flags] [Arguments]
    if(dictionary[command_id].flag_count > 0){
        print(dictionary[command_id].name);
        print("\n   Usage: ");
        print(dictionary[command_id].name);
        print(" [Flags] [Arguments]\n");
    } 

    //if no flags, print Command_name [Arguments]
    else {
        print(dictionary[command_id].name);
        print("\n   Usage: ");
        print(dictionary[command_id].name);
        print(" [Arguments]\n");
    }
                    
    print("   Description: ");
    print(dictionary[command_id].description);
    print("\n\n");

    //if has flags, print those out with their description and some formatting
    if(dictionary[command_id].flag_count > 0){
        print("   Flags:\n");
        for(int s = 0; s < dictionary[command_id].flag_count; s++){
            print("     ");
            print(dictionary[command_id].flags[s].flag);
            int name_len = kstrlen(dictionary[command_id].flags[s].flag);
            int padding_needed = 5 - name_len;
            for(int x = 0; x < padding_needed; x++){
                print(" ");
            }
            print(dictionary[command_id].flags[s].description);
            print("\n");

        }
    }
}

void cmd_help(parsed_command_t *cmd) {

    //if help is called on another command (arguments are present)
    if(cmd->arg_count > 0) {

        //iterate through which commands help was called on
        for(int i = 0; i < cmd->arg_count; i++){

            //iterate through list of commands to find a match
            for(int j = 0; j < COMMAND_COUNT; j++){
                if(kstrcmp(cmd->args[i], dictionary[j].name) == 0) {
                    cmd_help_print(j);
                }
            }
        }
    }
    else{
        for(int i = 0; i < COMMAND_COUNT; i++){
            cmd_help_print(i);
        }
    }
}

void cmd_clear(parsed_command_t *cmd){
    (void)cmd;
    clear_screen();
}