#include "main.h"

#define MAX_SIZE 512 //Max length of the input from the user in command line
#define PIPE 1
#define REDIR 2
#define CD 3
#define EXIT 4
#define PATH 5
#define ALIAS 6
#define MYHIST 7

int main(int argc, char **argv, char *envp[]){
    int mode = argc;                        // iterative boolean variable
    char* input = malloc(sizeof(char) * (512));    // will store input from user
    char** cmd_list = malloc(sizeof(char*) * 512);      // will store tokenized commands
    alias_node* alias_head = NULL;          // head for alias linked list
    myhistory_node* myhistory_head = NULL;  // head for myhistory linked list
    FILE* file_ptr;
    if(input == NULL || cmd_list == NULL)
    {
        perror("malloc failure");
        return 1;
    }
    if(mode == 2)
    {
        file_ptr = fopen(argv[1], "r");
        if(file_ptr == NULL)
        {
            printf("Error opening file at path: %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    }
    // loop will run during interactive mode
    while(mode != 0){
        if(mode == 1)   // prompt for user input
        {
            printf("newshell > ");                      // print shell icon
            if(fgets(input, 512, stdin) == NULL)   // get user input; check for errors
            {
                perror("failed getting user input");
                return 1;
            }
        }
        else if(mode == 2)  // scan for file input
        {
            if(fscanf(file_ptr, " %511[^;]", input) != 1)
            {
                printf("EOF reached\n");
                return 0;
            }
            int c = fgetc(file_ptr);
            if(c == EOF)
            {
                printf("EOF reached\n");
                return 0;
            }
            printf("Executing command: %s\n", input);
        }
        input[strcspn(input, "\n")] = '\0';   // remove trailing newline
        int command_count = separate_semicolons(input, cmd_list);
        for(int i=0; i<command_count; i++)
        {
            int cmd_type = command_type(cmd_list[i], alias_head);
            if(cmd_type != -1)
            {
                myhistory_head = myhistory_add_node(cmd_list[i], myhistory_head);    // add command to myhistory
            }
            switch(cmd_type)
            {
                case CD: {
                    char *exec_args[MAX_SIZE];
                    int exec_argc = 0;
                    char *token = strtok(cmd_list[i], " \t\n");
                    while (token != NULL && exec_argc < MAX_SIZE) {
                        exec_args[exec_argc++] = token;
                        token = strtok(NULL, " \t\n");
                    }
                    exec_args[exec_argc] = NULL;
                    cd_command(exec_args, exec_argc);// Simeon Childress
                    break;
                }
                case EXIT:{
                    free(input);
                    free_commands(cmd_list);
                    alias_remove_all(alias_head);
                    myhistory_remove_all(myhistory_head);
                    builtin_exit(); // Ali Akhtar
                    break;
                }
                case PATH:{
                    break;
                }
                case ALIAS:{
                    char* temp = malloc(sizeof(char) * 512);
                    strcpy(temp, cmd_list[i]);
                    temp = alias_execute(temp, alias_head);
                    if(temp != NULL)
                    {
                        strcpy(cmd_list[i], temp);
                    }
                    free(temp);
                    if(strcmp(cmd_list[i], "alias") == 0)
                    {
                        alias_show_all(alias_head);
                    }
                    else if(cmd_list[i][6] == '-')      // command contains flag
                    {
                        if(cmd_list[i][7] == 'r')
                        {
                            char* alias_name = malloc(sizeof(char) * 512);
                            strcpy(alias_name, cmd_list[i]+9);
                            printf("removing: %s\n", alias_name);
                            alias_head = alias_remove_name(alias_name, alias_head);
                            free(alias_name);
                        }
                        else if(cmd_list[i][7] == 'c')
                        {
                            alias_remove_all(alias_head);
                            alias_head = NULL;
                        }
                    }
                    else    // command is standard syntax
                    {
                        int eq_idx = -1;    // find idx of equals sign
                        for(int j=6; cmd_list[i][j] != '\0'; j++)
                        {
                            if(cmd_list[i][j] == '=')
                            {
                                eq_idx = j;
                                break;
                            }
                        }
                        char* alias_name = malloc(sizeof(char) * 512);
                        char* alias_command = malloc(sizeof(char) * 512);
                        strncpy(alias_name, cmd_list[i]+6, eq_idx-6);
                        strncpy(alias_command, cmd_list[i]+eq_idx+2, strlen(cmd_list[i])-(eq_idx)-3);
                        alias_head = alias_add_node(alias_name, alias_command, alias_head);
                    }
                    break;
                }
                case MYHIST:{
                    
                    if(strcmp(cmd_list[i], "myhistory") == 0)
                    {
                        myhistory_show_all(myhistory_head);
                    }
                    else if(cmd_list[i][11] == 'c')
                    {
                        myhistory_remove_all(myhistory_head);
                        myhistory_head = NULL;
                    }
                    else if(cmd_list[i][11] == 'e')
                    {
                        char* number = malloc(sizeof(char) * 512);
                        strcpy(number, cmd_list[i]+13);
                        int num = atoi(number);
                        char* myhistory_command = myhistory_execute(num, myhistory_head);   // command to be executed
                        printf("Executing command: %s\n", myhistory_command);
                        strcpy(cmd_list[i], myhistory_command);
                        i--;
                        free(number);
                    }
                    break;
                }
                case PIPE:{
                    Pipe* p = make_pipe(cmd_type, cmd_list[i]);
                    run_pipeline(p);
                    break;
                }
                case REDIR:{
                    char* in_file = NULL;
                    char* out_file = NULL;
                    char* command_part = redir_parse_command(cmd_list[i], &in_file, &out_file);
                    if (command_part != NULL) {
                        execute_with_redirection(command_part, in_file, out_file); // Simeon Childress
                        free(command_part);
                    }
                    if (in_file != NULL) free(in_file);
                    if (out_file != NULL) free(out_file);
                    break;
                }
                default:
                    printf("ERROR: Invalid command syntax\n");
            }
        }
    }
    fclose(file_ptr);
    free(input);
    free_commands(cmd_list);
    alias_remove_all(alias_head);
    myhistory_remove_all(myhistory_head);
    return 0;
}

int separate_semicolons(char* input, char** cmd_list){
    char* line = strdup(input);
    if(line == NULL)
    {
        return -1;
    }
    int token_idx = 0;
    char* token = strtok(line, ";");

    while(token != NULL && token_idx < 10)
    {
        while(*token == ' ')
        {
            token++;
        }
        cmd_list[token_idx] = strdup(token);
        token_idx++;
        token = strtok(NULL, ";");
    }
    free(line);
    return token_idx;   // return number of commands found
}

void free_commands(char** cmd){
    int i = 0;
    while(cmd[i] != NULL){
        free(cmd[i]);
        i++;
    }
}

int command_type(char* cmd, alias_node* alias_head)
{
    if(alias_head != NULL)
    {
        char* temp = alias_execute(cmd, alias_head);
        if(temp != NULL)
        {
            strcpy(cmd, temp);
        }
    }
    regex_t pipe_syntax, redir_syntax, cd_syntax, exit_syntax, path_syntax, alias_syntax, myhistory_syntax;
    int pipe_value = regcomp(&pipe_syntax, "^[^|]+( \\| [^|]+)( \\| [^|]+)?$", REG_EXTENDED);
    if(pipe_value == 0)
    {
        pipe_value = regexec(&pipe_syntax, cmd, 0, NULL, 0);
        if(pipe_value == 0)
        {
            return PIPE;
        }
    }
    int redir_value = regcomp(&redir_syntax, "^(.+?)\\s*[<>]\\s*[^<> ]+$", REG_EXTENDED);
    //int redir_value = regcomp(&redir_syntax, "^([^<> ]+ [<>] [^<> ]+)$", REG_EXTENDED);
    if(redir_value == 0)
    {
        redir_value = regexec(&redir_syntax, cmd, 0, NULL, 0);
        if(redir_value == 0)
        {
            return REDIR;
        }
    }
    int cd_value = regcomp(&cd_syntax, "^(cd)(\\s+[^\\s]+)?$", REG_EXTENDED);
    //int cd_value = regcomp(&cd_syntax, "^(cd)( .{0,2}(/[a-zA-z]*)+)?$", REG_EXTENDED);
    if(cd_value == 0)
    {
        cd_value = regexec(&cd_syntax, cmd, 0, NULL, 0);
        if(cd_value == 0)
        {
            return CD;
        }
    }
    int exit_value = regcomp(&exit_syntax, "^(exit)$", REG_EXTENDED);
    if(exit_value == 0)
    {
        exit_value = regexec(&exit_syntax, cmd, 0, NULL, 0);
        if(exit_value == 0)
        {
            return EXIT;
        }
    }
    int path_value = regcomp(&path_syntax, "^(path)(( +| -)( .{0,2}(/[a-zA-z]*)+))?$", REG_EXTENDED);
    if(path_value == 0)
    {
        path_value = regexec(&path_syntax, cmd, 0, NULL, 0);
        if(path_value == 0)
        {
            return PATH;
        }
    }
    int alias_value = regcomp(&alias_syntax, "^(alias)( [a-zA-z]+(='[^']+')| -c| -r [a-zA-z]+)?$", REG_EXTENDED);
    if(alias_value == 0)
    {
        alias_value = regexec(&alias_syntax, cmd, 0, NULL, 0);
        if(alias_value == 0)
        {
            return ALIAS;
        }
    }
    int myhistory_value = regcomp(&myhistory_syntax, "^(myhistory)( -c| -e [0-9]+)?$", REG_EXTENDED);
    if(myhistory_value == 0)
    {
        myhistory_value = regexec(&myhistory_syntax, cmd, 0, NULL, 0);
        if(myhistory_value == 0)
        {
            return MYHIST;
        }
    }
    return -1;
}
