#include <stdio.h>
#include "primes.h"

#define MAX 2000000

int fulfills_goldbach_theory(int n) {
  int i;
  for (i = 1; i < n; i += 2) {
    if (is_prime(i)) {
    }
  }
  return 0;
}

int main(void) {
  int i;

  printf("Test of Goldbach!\n");

  for (i = 7; i < MAX; i++) {
    if (!fulfills_goldbach_theory(i)) {
      printf("Theory failed!\n");
      break;
    }
  }

  printf("Finished!");

  return 0;
}

