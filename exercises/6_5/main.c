#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void scan_data(char *operator, double *right_operand);
void do_next_op(char operator, double right_operand, double *accumulator);

int main(void) {
  char operator;
  double right_operand, accumulator = 0;

  do {
    scan_data(&operator, &right_operand);
    do_next_op(operator, right_operand, &accumulator);

    if (operator == 'q')
      printf("final result is ");
    else
      printf("result so far is ");

    printf("%.1f\n", accumulator);

  } while (operator != 'q');

  return 0;
}

void scan_data(char *operator, double *right_operand) {
  scanf(" %c %lf", operator, right_operand);
}

void do_next_op(char operator, double right_operand, double *accumulator) {
  switch (operator) {
    case '+': *accumulator += right_operand; break;
    case '-': *accumulator -= right_operand; break;
    case '*': *accumulator *= right_operand; break;
    case '^': *accumulator = pow(*accumulator, right_operand); break;
    case '/':
      if (right_operand != 0)
        *accumulator /= right_operand;
      break;
  }
}
