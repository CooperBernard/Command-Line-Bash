// Cooper Bernard | crb0441

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct alias_node{
    char* name;            // stores the user-defined alias for the command
    char* command;         // stores the command that the user has created an alias for
    struct alias_node* next;    // points to next node in linked list
}alias_node;

alias_node* alias_create_node(char* name, char* command);                   // creates new node

alias_node* alias_add_node(char* name, char* command, alias_node* head);    // adds node to linked list; returns head

void alias_show_all(alias_node* head);                                      // prints all alias commands the user has made

alias_node* alias_remove_name(char* name, alias_node* head);                // removes user specified alias from list; returns head

void alias_remove_all(alias_node* head);                                    // removes all stored aliases

char* alias_execute(char* name, alias_node* head);                          // returns command associated with alias
