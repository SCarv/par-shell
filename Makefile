# Makefile for par-shell.

CFLAGS=-pthread -Wall -Werror -g
# Flags detailed:
# pthread: enable multithreading
# Wall: All warnings.
# Werror: Warnings become errors.
# g: enable debugging symbols

all: par-shell par-shell-in

par-shell-in: par-shell-in.o par_sync.o
	$(CC) $(CFLAGS) -o par-shell-in par-shell-in.o par_sync.o

par-shell-in.o: par-shell-in.c 
	$(CC) $(CFLAGS) -c -o par-shell-in.o par-shell-in.c

par-shell: list.o par_sync.o par_wait.o main.o 
	$(CC) $(CFLAGS) -o par-shell list.o par_sync.o par_wait.o main.o

main.o: main.c par_sync.h list.h
	$(CC) $(CFLAGS) -c -o main.o main.c

par_sync.o: par_sync.c par_wait.h par_sync.h list.h
	$(CC) $(CFLAGS) -c -o par_sync.c

par_wait.o: par_wait.c par_wait.h par_sync.h
	$(CC) $(CFLAGS) -c -o par_wait.o par_wait.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

clean:
	rm *.o

fibonacci: fibonacci.c
	$(CC) fibonacci.c -o fibonacci

remove: 
	rm par-shell fibonacci

test: par-test.sh fibonacci par-shell
	./par-test.sh

delete:
	rm fibonacci par-shell
