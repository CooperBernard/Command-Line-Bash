/*
 * Anonymous
 * Description: Implements input and output redirection for external shell commands.
 */

 #include <stdio.h>      // For perror
 #include <stdlib.h>     // For exit
 #include <string.h>     // For string functions
 #include <unistd.h>     // For fork, execvp, dup2
 #include <fcntl.h>      // For open
 #include <sys/wait.h>   // For wait
 #include "new-redirection.h"
 
 #define MAX_SIZE 512
 
 // Executes a command with optional input (<) or output (>) redirection
 // Only one redirection is allowed at a time (input OR output)
 // args[] must NOT include the < or > symbols or filenames
 
 // Helper function to tokenize command string into execvp-style args
 void tokenize_command(char* command_string, char* args[]) {
     int argc = 0;
     char* token = strtok(command_string, " \t\n");
     while (token != NULL && argc < MAX_SIZE - 1) {
         args[argc++] = token;
         token = strtok(NULL, " \t\n");
     }
     args[argc] = NULL;
 }
 
 // Executes a command with optional input (<) or output (>) redirection
 void execute_with_redirection(char *command_string, char *input_file, char *output_file) {
     char* args[MAX_SIZE];
     tokenize_command(command_string, args);  // Tokenize before passing to execvp
 
     pid_t pid = fork();
     if (pid == 0) { // Child process
         // Input redirection
         if (input_file != NULL) {
             int fd_in = open(input_file, O_RDONLY);
             if (fd_in < 0) {
                 perror("open (input file)");
                 exit(1);
             }
             dup2(fd_in, STDIN_FILENO);
             close(fd_in);
         }
 
         // Output redirection
         if (output_file != NULL) {
             int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
             if (fd_out < 0) {
                 perror("open (output file)");
                 exit(1);
             }
             dup2(fd_out, STDOUT_FILENO);
             close(fd_out);
         }
 
         // Run the external command
         execvp(args[0], args);
         perror("execvp");
         exit(1);
 
     } else if (pid > 0) { // Parent process
         wait(NULL); // Wait for child to finish
     } else {
         perror("fork");
     }
 }
 
// Parses the redirect command string and sets input/output files
// Only supports single redirection at a time (either < or >)
// Returns the command portion of the input string
char* redir_parse_command(char* cmd, char** in_file, char** out_file) {
    char redir_sign = '\0';
    int sign_idx = -1;

    // Find redirection symbol position and type
    for (int i = 0; i < strlen(cmd); i++) {
        if (cmd[i] == '>' || cmd[i] == '<') {
            sign_idx = i;
            redir_sign = cmd[i];
            break;
        }
    }

    if (sign_idx == -1) {
        printf("ERROR: No redirection symbol found\n");
        *in_file = NULL;
        *out_file = NULL;
        return NULL;
    }

    // Free previous pointers if they exist
    if (*in_file != NULL) free(*in_file);
    if (*out_file != NULL) free(*out_file);

    char* command_part = strndup(cmd, sign_idx);  // The left side (the command itself)
    char* file_part = cmd + sign_idx + 1;         // The right side (the filename)
    while (*file_part == ' ') file_part++;        // Trim leading spaces

    if (redir_sign == '>') {
        *out_file = strdup(file_part);
        *in_file = NULL;
    } else if (redir_sign == '<') {
        *in_file = strdup(file_part);
        *out_file = NULL;
    } else {
        printf("ERROR: Invalid redirect syntax\n");
        free(command_part);
        *in_file = NULL;
        *out_file = NULL;
        return NULL;
    }

    return command_part;  // Return the actual command back to main
}
