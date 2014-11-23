#include <stdio.h>

int quotient(int dividend, int divisor) {
  if (dividend >= divisor) 
    return 1 + quotient(dividend - dividend, divisor);
  return 0;
}

int modulus(int dividend, int divisor) {
  if (dividend < divisor)
    return dividend;
  return modulus(dividend-divisor, divisor);
}

int main(void) {
  printf("Quotient 6/3 = %d\n", quotient(6,3));
  printf("Quotient 13/3: %d\n", quotient(13,3));
  printf("Modulus 12 mod 4: %d\n", modulus(12,4));
  printf("Modulus 729 mod 45: %d\n", modulus(729,3));
  return 0;
}
