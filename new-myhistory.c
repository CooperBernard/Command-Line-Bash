// Cooper Bernard

#include "new-myhistory.h"

myhistory_node* myhistory_create_node(char* command)
{
    myhistory_node* new_node = (myhistory_node*)malloc(sizeof(myhistory_node));     // allocate memory for new node

    if(new_node == NULL)                                    // check if malloc failed
    {
        fprintf(stderr, "Failed to create new node\n");     // print appropriate message
        exit(EXIT_FAILURE);                                 // exit program
    }

    new_node->command = strdup(command);       // assign data to node
    new_node->next = NULL;              // assign next ptr

    return new_node;                    // return new node
}

myhistory_node* myhistory_add_node(char* command, myhistory_node* head)
{
    myhistory_node* new_node = myhistory_create_node(command);      // create new node
    if(head == NULL)        // if the list has not been created
    {
        head = new_node;    // assign node to head
        return head;        // return new head
    }

    int idx = 0;                    // iterator for list size
    myhistory_node* temp = head;    // create temp helper node
    while(temp->next != NULL)     // iterate over list
    {
        temp = temp->next;  // iterate to next node
        idx++;              // increment index tracker
    }
    if(idx == 19)                           // if list is full
    {                                       // remove first node
        myhistory_node* extra_node = head; // create temp helper   
        head = head->next;              // reassign head node
        free(extra_node->command);          // free extra node from memory
        free(extra_node);                   // free extra node from memory
    }
    temp->next = new_node;                              // assign node to tail
    return head;
}

void myhistory_show_all(myhistory_node* head)
{
    myhistory_node* current = head; // create helper node
    if(head == NULL)    // if the list has not been created
    {
        printf("ERROR: No commands have been entered.\n");  // print appropriate message
        return;
    }
    // print 20 most recent commands
    int idx=0;
    while(current != NULL)    // iterate over list
    {
        printf("%d\t%s\n", idx, current->command);  // print list of commands
        current = current->next;          // iterate to next node
        idx++;
    }
}

void myhistory_remove_all(myhistory_node* head)
{
    myhistory_node* current = head;    
    while(current != NULL)   // iterate over list
    {
        myhistory_node* temp = current;     // stores node in temp
        current = current->next;            // remove temp from chain
        free(temp->command);
        free(temp);                         // free temp from memory
    }
}

char* myhistory_execute(int idx, myhistory_node* head)
{
    myhistory_node* temp = head;    // create temp node
    for(int i=0; i<idx && temp != NULL; i++)    // iterate until desired node is reached or end of list
    {
        temp = temp->next;
    }
    if(temp == NULL)    // check if index is too large
    {
        printf("ERROR: Index too large for command history\n");     // print suitable message
        return NULL;
    }
    return temp->command;   // return appropriate command to be executed
}