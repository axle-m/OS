#include "runner.h"
#include "lexer.h"
#include "../../libk/include/kstring.h"

extern void putchar(char c);
extern void print(const char *str);
extern void clear_screen();
extern const commant_t dictionary[];
#define COMMAND_COUNT (sizeof(dictionary) / sizeof(dictionary[0])

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
                for(int j = 0; j < cmd->arg_count; i++){
                    print(cmd->args[j]);
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
        print("Usage: ");
        print(dictionary[command_id].name);
        print(" [Flags] [Arguments]\n");
    } 

    //if no flags, print Command_name [Arguments]
    else {
        print("Usage: ");
        print(dictionary[command_id].name);
        print(" [Arguments]\n");
    }
                    
    print("Description: ");
    print(dictionary[command_id].description);
    print("\n\n");

    //if has flags, print those out with their description and some formatting
    if(dictionary[command_id].flag_count > 0){
        print("Flags:\n");
        for(int s = 0; s < dictionary[command_id].flag_count; s++){
            print("   ");
            print(dictionary[command_id].flags[s].name);
            int name_len = kstrlen(dictionary[command_id].flags[s].name);
            int padding_needed = 8 - name_len;
            for(int x = 0; x < padding_needed; x++){
                print(" ");
            }
            print(dictionary[command_id].flags[s].description);

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
                if(kstrcmp(cmd->args[i], dictionary[j]) == 0) {
                    cmd_help_print(j);
                }
            }
        }
    }
    else{
        for(int i = 0; i < COMMAND_COUNT; i++){
            cmd_help_print[i];
        }
    }
}

void clear(parsed_command_t *cmd){
    clear_screen();
}