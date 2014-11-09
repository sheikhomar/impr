build: main.o primes.o
	gcc -ansi -Wall -pedantic -o program main.o primes.o -lm
main.o: main.c primes.h
	gcc -ansi -Wall -pedantic -c main.c -lm
primes.o: primes.c primes.h
	gcc -ansi -Wall -pedantic -c primes.c -lm
clean:
	rm *.o *.gch program
