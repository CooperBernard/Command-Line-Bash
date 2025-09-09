#ifndef IO_REDIRECTION_H
#define IO_REDIRECTION_H

// Executes an external command with optional input or output redirection
void execute_with_redirection(char *command_string, char *input_file, char *output_file);

// Parses a command string and extracts input/output filenames (if any)
// Returns the command portion (left of the redirection symbol)
char* redir_parse_command(char* cmd, char** in_file, char** out_file);

#endif
