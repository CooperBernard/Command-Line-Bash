#include <stdlib.h>
#include <stdio.h>
#include "new-exit.h"


void builtin_exit() {
    printf("Exiting safely\n");
    exit(0);
}
