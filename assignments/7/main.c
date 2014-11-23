#include <stdio.h>
#define RESULT_ARRAY_SIZE 8

void merge(double *result, int result_size, const double *a, int a_size, const double *b, int b_size) {
  int result_index = 0, a_index = 0, b_index = 0;

  while (result_index < result_size) {
    if (a_index < a_size && b_index < b_size) {
      if (a[a_index] < b[b_index]) {
        result[result_index] = a[a_index];
        a_index++;
      } else if (b[b_index] < a[a_index]) {
        result[result_index] = b[b_index];
        b_index++;
      } else {
        result[result_index] = b[b_index];
        b_index++;
        a_index++;
      }
    } else if (a_index < a_size) {
      result[result_index] = a[a_index];
      a_index++;
    } else if (b_index < b_size) {
      result[result_index] = b[b_index];
      b_index++;
    }
    result_index++;
  }
}

int main(void) {
  int i;
  double first[] = {-10.5, -1.8, 3.5, 6.3, 7.2};
  double second[] = {-1.8, 3.1, 6.3};
  double result[RESULT_ARRAY_SIZE];

  merge(result, RESULT_ARRAY_SIZE, first, 5, second, 3);

  for (i = 0; i < RESULT_ARRAY_SIZE; i++)
    printf("%.1f\n", result[i]);

  return 0;
}
