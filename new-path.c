//Anonymous
#include <stdlib.h>
#include <stdio.h>
#include "new-path.h"


void print_env_path(char **env, char **environ){
    for (env = environ; *env; ++env) {
        printf("%s\n", *env);
    }
}
