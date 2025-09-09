#ifndef NEW_PIPELINING_H
#define NEW_PIPELINING_H


typedef struct {
    int type;
    char* left;
    char* right;
}Pipe;

Pipe* make_pipe(int type, char* line); // Cooper Bernard

void run_pipeline(Pipe *p);

#endif
