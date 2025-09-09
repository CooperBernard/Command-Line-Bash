// Cooper Bernard | crb0441

#include "new-alias.h"

alias_node* alias_create_node(char* name, char* command)
{
    alias_node* new_node = (alias_node*)malloc(sizeof(alias_node));     // allocate memory for new node

    if(new_node == NULL)                                    // check if malloc failed
    {
        fprintf(stderr, "Failed to create new node\n");     // print appropriate message
        exit(EXIT_FAILURE);                                 // exit program
    }

    new_node->name = strdup(name);             // assign data to node
    new_node->command = strdup(command);       // assign data to node
    new_node->next = NULL;              // assign next ptr

    return new_node;                    // return new node
}

alias_node* alias_add_node(char* name, char* command, alias_node* head)
{
    struct alias_node* new_node = alias_create_node(name, command);   // create new node
    if(head == NULL)    // if the list has not been created
    {
        head = new_node;    // assign node to head
        return head;
    }

    alias_node* current = head;     // create helper node
    while(current->next != NULL)    // iterate over list until tail node is found
    {
        if(strcmp(current->name, name)==0)   // if duplicate node found
        {
            printf("ERROR: Alias already exists with the name: %s\n", name);   // print appropriate message
            return head;
        }
        current = current->next;    // iterate to next node
    }
    current->next = new_node;   // assign node to tail
    return head;
}

void alias_show_all(alias_node* head)
{
    alias_node* current = head; // create helper node
    if(head == NULL)    // if the list has not been created
    {
        printf("ERROR: No aliases have been created.\n");  // print appropriate message
        return;
    }
    // print all aliases and their associated commands
    printf("Aliases:\n");
    while(current != NULL)    // iterate over list
    {
        printf("%s -> %s\n", current->name, current->command);  // print list of aliases
        current = current->next;                                // iterate to next node
    }
}

alias_node* alias_remove_name(char* name, alias_node* head)
{
    if(head == NULL)    // if list does not exist
    {
        printf("ERROR: No aliases have been created.\n");   // print appropriate message
        return NULL;
    }
    alias_node* current = head;     // create helper node
    alias_node* temp = NULL;        // create helper node

    // special case; remove head node
    if(strcmp(current->name, name) == 0)
    {
        alias_node* new_head = head->next;
        free(current->name);
        current->name = NULL;
        free(current->command);
        current->command = NULL;
        free(head);
        return new_head;
    }

    while(current != NULL && strcmp(current->name, name) != 0)
    {
        temp = current;
        current = current->next;
    }

    if(current == NULL)
    {
        printf("ERROR: No alias was found with the name: %s\n", name);
        return head;
    }

    temp->next = current->next;
    free(current->name);
    free(current->command);
    free(current);
    return head;
}

void alias_remove_all(alias_node* head)
{
    alias_node* current = head;    
    while(current != NULL)   // iterate over list
    {
        alias_node* temp = current;     // stores node in temp
        current = current->next;        // remove temp from chain
        free(temp->name);
        free(temp->command);
        free(temp);                     // free temp from memory
    }
}

char* alias_execute(char* name, alias_node* head)
{
    alias_node* current = head;     // create iterator node
    while(current != NULL)      // iterate until end of list
    {
        if(strcmp(current->name, name) == 0)    // check for matching alias
        {
            return current->command;    // return command to be executed
        }
        current = current->next;
    }
    // end of list reached
    return NULL;
}
