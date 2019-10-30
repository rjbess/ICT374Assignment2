main: main.o token.o command.o
	gcc main.c -o main -Wall -pedantic -std=c11
token.o: token.c token.h
	gcc -c token.c
command.o: command.c command.h
	gcc -c command.c
clean:
	rm *.o
