/*
 * sgc0110: Simeon Childress
 * Course: CSCE 3600
 * Description: Implements the built-in 'cd' command for the shell.
 */

#include <stdio.h>      // For printf, fprintf, perror
#include <stdlib.h>     // For getenv
#include <string.h>     // For string functions
#include <unistd.h>     // For chdir
#include "new-cd.h"

#define MAX_ARGS 10

// Built-in 'cd' command
// ---------------------
// Changes the current working directory.
// - If no path is given, changes to the HOME directory.
// - If a path is given, changes to that path.
// - If more than one argument is given, prints an error.
int cd_command(char *args[], int argc) {
    const char *path;

    // Too many arguments
    if (argc > 2) {
        fprintf(stderr, "cd: too many arguments\n");
        return -1;
    }

    // No path provided — go to HOME
    if (argc == 1) {
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, "cd: HOME environment variable not set\n");
            return -1;
        }
    } else {
        // Use provided path
        path = args[1];
    }

    // Try to change directory
    if (chdir(path) != 0) {
        perror("cd");
        return -1;
    }

    return 0;
}

/* Temporary main for testing the cd_command function
int main(int argc, char *argv[]) {
    return cd_command(argv, argc);
}
*/
