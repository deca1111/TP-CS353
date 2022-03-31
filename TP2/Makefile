all: main.o liste.o
	gcc main.o liste.o -o exec

main.o : main.c liste.h
	gcc -c main.c -o main.o

liste.o : liste.c
	gcc -c liste.c -o liste.o

clean :
	rm *.o exec
