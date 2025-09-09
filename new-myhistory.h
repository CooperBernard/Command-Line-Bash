// Cooper Bernard

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct myhistory_node{
    char* command;             // stores the command the user has entered
    struct myhistory_node* next;    // points to next node in linked list
}myhistory_node;

myhistory_node* myhistory_create_node(char* command);                       // creates new node

myhistory_node* myhistory_add_node(char* command, myhistory_node* head);   // adds new node to linked list; max of 20 before data overwrite; returns head node

void myhistory_show_all(myhistory_node* head);                              // prints 20 most recent commands user has entered

void myhistory_remove_all(myhistory_node* head);                            // removes all nodes in list

char* myhistory_execute(int idx, myhistory_node* head);                     // returns command to be executed from user index
