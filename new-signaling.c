//Anonymous
#include "new-signaling.h"
//terminate_group() creates a single subprocess for the shell, and will prevent the parent shell being terminated by a signal
pid_t terminate_group() {
    pid_t pid = fork();
    if (pid == 0) {//child
        setpgid(0, 0);                  // Put self in a new process group
        tcsetpgrp(STDIN_FILENO, getpid());
        return getpid();  // Sets the current fork() as the FG
    }else if(pid < 0){
        perror("Fork Error\n");
    } else { //parent
        setpgid(pid, pid);              // Ensure child is in its own process group
        tcsetpgrp(STDIN_FILENO, pid);    // Child controls the terminal.
    }
    return pid;
}
