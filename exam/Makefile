build: main.c
	gcc -ansi -Wall -pedantic main.c
debug: main.c
	gcc -ansi -Wall -pedantic -g -O0 main.c
valgrind: debug
	valgrind -v --leak-check=yes ./a.out --print
