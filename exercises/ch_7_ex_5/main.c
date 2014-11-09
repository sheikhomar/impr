#include <stdio.h>
#include <stdlib.h>

int sum_odd_numbers(int *numbers, int array_size) {
  int i, sum = 0;

  for(i = 0; i < array_size - 1; i += 2)
    sum += numbers[i];

  return sum;
}

int sum_even_numbers(int *numbers, int array_size) {
  int i, sum = 0;

  for(i = 1; i < array_size - 1; i += 2)
    sum += numbers[i];

  return sum;
}

int main(void) {
  int barcode[12] = {0, 7, 9, 4, 0, 0, 8, 0, 4, 5, 0, 1};
  int sum1, sum2, last_digit, check_digit;

  sum1 = sum_odd_numbers(barcode, 12);
  sum2 = sum_even_numbers(barcode, 12);
  last_digit = (sum1 * 3 + sum2) % 10;
  check_digit = (last_digit == 0 ? 0 : 10 - last_digit);

  if (check_digit == barcode[11]) {
    printf("Barcode validated!\n");
  } else {
    printf("Barcode invalid!\n");
  }

  return 0;
}
