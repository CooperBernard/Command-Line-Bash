/*command
$ cmd1 | cmd2 | cmd3
connects the standard output of cmd1 to the standard input of cmd2, and again
connects the standard output of cmd2 to the standard input of cmd3 using the
pipeline operator '|'.
You will need to use the pipe() system call. Your shell should be able to
handle up to three commands chained together with the pipeline operator (i.e.,
your shell should support up to two pipes pipelined together). This portion of
the project should only require implementing support for a pipeline of 2 pipes/3
commands (no working with redirection).
5 of 9
Your shell does not need to handle built-in commands implemented above (i.e.,
cd, exit, path, and myhistory) in pipeline.*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "new-pipelining.h"

// Cooper Bernard | crb0441
Pipe* make_pipe(int type, char* line)
{
    Pipe* new_pipe = (Pipe*)malloc(sizeof(Pipe));

    if(new_pipe == NULL)
    {
        fprintf(stderr, "Failed to create new pipe struct\n");  // print appropriate message
        exit(EXIT_FAILURE);                                     // exit program
    }

    int pipe_idx = -1;
    for(int i=0; i<strlen(line); i++)
    {
        if(line[i] == '|')
        {
            pipe_idx = i;
            break;
        }
    }
    char* left = malloc(sizeof(char) * 512);
    char* right = malloc(sizeof(char) * 512);
    strncpy(left, line, pipe_idx-2);
    strncpy(right, line+pipe_idx+1, strlen(line)-pipe_idx-2);
    new_pipe->left = strdup(left);
    new_pipe->right = strdup(right);
    new_pipe->type = type;
    free(left);
    free(right);
    return new_pipe;
}

void run_pipeline(Pipe *p) {
    char *args1[10], *args2[10];
    int i = 0, j = 0;

    
    char *token = strtok(p->left, " \t\n");
    while (token != NULL && i < 9) {
        args1[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args1[i] = NULL;

    
    token = strtok(p->right, " \t\n");
    while (token != NULL && j < 9) {
        args2[j++] = token;
        token = strtok(NULL, " \t\n");
    }
    args2[j] = NULL;

    int pipefd[2];
    pipe(pipefd);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO);  // redirect stdout to pipe
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(args1[0], args1);
        perror("execvp failed (left command)");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(pipefd[0], STDIN_FILENO);   // redirect stdin from pipe
        close(pipefd[1]);
        close(pipefd[0]);
        execvp(args2[0], args2);
        perror("execvp failed (right command)");
        exit(1);
    }

    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}
