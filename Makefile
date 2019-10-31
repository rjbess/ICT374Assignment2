main: main.o token.o command.o menu.o
	gcc main.o token.o command.o menu.o -o main -Wall -pedantic -std=c11
main.o: main.c
	gcc -c main.c
token.o: token.c token.h
	gcc -c token.c
command.o: command.c command.h
	gcc -c command.c
menu.o: menu.c menu.h
	gcc -c menu.c
clean:
	rm *.o
