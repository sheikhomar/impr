#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LIST_SIZE 100

int pseudo_random_int(int min, int max) {
  return (rand() % max) + min;
}

void print_numbers(double *numbers, int list_size) {
  int i;
  for(i = 0; i < list_size; i++) {
    if (i != 0 && i % 10 == 0) {
      printf("\n");
    }
    printf("%10.0f ", numbers[i]);
  }
  printf("\n");
}

int double_comparer(const void *input1, const void *input2) {
  double *element1 = (double*)input1;
  double *element2 = (double*)input2;

  return *element1 - *element2;
}

int main(void) {
  int i;
  double *list_of_doubles = malloc(LIST_SIZE * sizeof(double));

  srand(time(NULL));

  for(i = 0; i < LIST_SIZE; i++) {
    list_of_doubles[i] = pseudo_random_int(100, 4000);
  }

  printf("Before qsort:\n");
  print_numbers(list_of_doubles, LIST_SIZE);

  qsort(list_of_doubles, LIST_SIZE, sizeof(double), double_comparer);

  printf("After qsort:\n");
  print_numbers(list_of_doubles, LIST_SIZE);

  free(list_of_doubles);

  return 0;
}
