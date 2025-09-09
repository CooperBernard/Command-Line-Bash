OBJS	= main.o new-alias.o new-cd.o new-exit.o new-myhistory.o new-path.o new-pipelining.o new-redirection.o new-signaling.o
SOURCE	= main.c new-alias.c new-cd.c new-exit.c new-myhistory.c new-path.c new-pipelining.c new-redirection.c new-signaling.c
HEADER	= main.h new-alias.h new-cd.h new-exit.h new-myhistory.h new-path.h new-pipelining.h new-redirection.h new-signaling.h
OUT	= newshell
FLAGS	= -g -c -Wall
CC	= gcc

all:	newshell

newshell: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c

new-alias.o: new-alias.c
	$(CC) $(FLAGS) new-alias.c

new-cd.o: new-cd.c
	$(CC) $(FLAGS) new-cd.c

new-exit.o: new-exit.c
	$(CC) $(FLAGS) new-exit.c

new-myhistory.o: new-myhistory.c
	$(CC) $(FLAGS) new-myhistory.c

new-path.o: new-path.c
	$(CC) $(FLAGS) new-path.c

new-pipelining.o: new-pipelining.c
	$(CC) $(FLAGS) new-pipelining.c

new-redirection.o: new-redirection.c
	$(CC) $(FLAGS) new-redirection.c

new-signaling.o: new-signaling.c
	$(CC) $(FLAGS) new-signaling.c

clean:
	rm -f $(OBJS) $(OUT)

