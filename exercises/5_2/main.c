#include <stdio.h>
#include "primes.h"

int main(void) {
  int counter = 1, i;

  for (i = 0; i <= 541; i++) {
    if (is_prime(i)) {
      printf("prime %d: %d\n", counter, i);
      counter++;
    }
    if (counter == 101)
      break;
  }
}

