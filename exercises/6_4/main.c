#include <stdio.h>

double add_one(double x) {
  return x + 1;
}

double half(double x) {
  return x / 2;
}
double dbl(double x) {
  return x * 2;
}

double multi_apply(double (*f)(double), int n, double s) {
  assert(n >= 1);

  if (n == 1)
    return f(s);
  
  return f(multi_apply(f, n-1, s));
}

double multi_apply_2(double (*f)(double), double (*g)(double), int n, double s) {
  if (n == 1)
    return f(g(s));
  
  return f(g(multi_apply(f, n-1, s)));
}

int main(void) {
  double result;

  result = multi_apply(half, 4, 30);
  printf("multi_apply result is %f\n", result);

  result = multi_apply_2(half, dbl, 3, 100);
  printf("multi_apply_2 result is %f\n", result);

  return 0;
}
