#include <stdio.h>
#include <time.h>
#define ARRAY_SIZE 45

long fib(int n){
  long result;

  if (n == 0)
    result = 0;
  else if (n == 1)
    result = 1;
  else
    result = fib(n-1) + fib(n-2);

  return result;
}

long fib_recursive(int n){
  long result;
  static long map[ARRAY_SIZE];

  if (n == 0)
    result = 0;
  else if (n == 1)
    result = 1;
  else
    result = fib(n-1) + fib(n-2);

  return result;
}

int main(void) {
  int i;
  time_t start_time;

  start_time = time(NULL);

  for(i = 1; i < 45; i++) {
    printf("fib(%i)=%li\n", i, fib(i));
  }

  printf("Program run in %d seconds\n", time(NULL) - start_time);
  printf("Current time: %d \n", time(NULL));

  return 0;
}
