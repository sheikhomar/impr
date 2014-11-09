build: main.c translate_seconds.o
	gcc -ansi -Wall -pedantic main.c translate_seconds.o
test: CuTest.o translate_seconds.o
	gcc -ansi -Wall -pedantic CuTest.o translate_seconds.o all_tests.c -o test_runner
clean:
	rm *.o *.gch *.out
translate_seconds.o: translate_seconds.c translate_seconds.h
	gcc -ansi -Wall -pedantic translate_seconds.h translate_seconds.c -c
CuTest.o: CuTest.h CuTest.c
	gcc -ansi -Wall -pedantic CuTest.h CuTest.c -c
