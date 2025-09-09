#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <regex.h>
#include "new-alias.h"
#include "new-cd.h"
#include "new-exit.h"
#include "new-myhistory.h"
#include "new-path.h"
#include "new-pipelining.h"
#include "new-redirection.h"
#include "new-signaling.h"

typedef struct{
    int type;
    void *data;
}Command;

typedef struct {
    int type;
    char file[100]; //full command for input
}Exec;



int command_type(char* cmd, alias_node* alias_head);        // returns appropriate command value, -1 if invalid

int separate_semicolons(char* input, char** cmd_list);      // takes input and stores lines separated by semicolons in cmd_list

void free_commands(char** cmd);      // frees all pointers in pointer array

int cd_command(char *args[], int argc);

