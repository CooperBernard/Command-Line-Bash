# Command-Line-Bash
Group project for my term final in Intro to System Programming

Major Assignment 2 - Group 13\
Cooper Bernard and 3 peers

Design Overview:
    
The program is intended to be a bash interface within the command line to execute simple commands typed by the user, following specific syntax. The program can also execute commands from a batch file, provided the file path is given as a command line argument.

The program takes a string input from the user or file, which is then passed into a function that returns what command must be executed. The program then determines which functions to call based on the input.

Overall, the program is designed with simplicity and ease of use in mind, so long as the user or file follows the correct syntax for any given command.

Responsibilities:

Add a new built in cd command (new-cd.c): Simeon Childress\
Add a new built in exit command (new-exit.c): Ali Akhtar\
Add a new built-in path command (new-path.c): Wesley Boyle\
Add a new built-in myhistory command (new-myhistory.c): Cooper Bernard

Extend your shell with I/O redirection (IO_redirection.c): Simeon Childress\
Extend your shell with pipelining (newshell_pipelining.c): Ali Akhtar\
Support Signal Handling and Terminal Control (SignalHandling_TerminalControl.c): Wesley Boyle\
Implement a new alias command (new_Alias.c): Cooper Bernard

Complete Specification:

The program uses a function to parse input, extracting each command from a single line and discarding semicolons. The program will either pull data from the file, or prompt the user for input. Afterwards, the program will verify that the syntax is correct, and if it is, it will execute the command given before moving onto the next. If the syntax is not correct, the program will skip the command and either prompt for another input, or attempt to execute the next command, if multiple commands were entered on the same line.

In batch mode, each command MUST be followed by a semicolon with no whitespace, including commands that are at the end of each line. Otherwise, the program will fail to parse and not execute the command.

e.g. of desired syntax
alias name='myhistory'; alias; myhistory;
myhistory -e 1;
alias -c;

Known Bugs or Problems:
    Several of the intended functionalities do not work for various reasons. The only functionalities that have been tested rigorously are: batch mode, alias, myhistory, and exit.
    cd is incompatible due to lack of PATH initialization.
    I/O redirection is not usable due to lack of user input parsing.
    Not compatible with piping or redirecting.
    There is no use of system calls such as fork() or exec() for certain commands.
