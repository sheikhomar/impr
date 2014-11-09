#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef double (*math_fn)(double);

double trap(double a, double b, int n, math_fn f);
double sum(double a, double b, int n, math_fn f);
double g(double x);
double h(double x);

int main(void) {
  int n;
  double area_h, area_g;

  for (n = 2; n <= 128; n+=n) {
    area_g = trap(0, 3.14159, n, &g);
    area_h = trap(-2, 2, n, &h);

    printf("When n=%3d  |  ", n);
    printf("g(x) area approximated: %.2f  |  ", area_g);
    printf("h(x) area approximated: %.2f\n", area_h);
  }

  return 0;
}

/* Implementation of the trapezoidal rule, which is a method for
 * approximating the definite integral
 */
double trap(double a, double b, int n, math_fn f) {
  double h = (b-a)/n;
  return (h/2) * (f(a) + f(b) + 2 * sum(a, b, n, f));
}

double sum(double a, double b, int n, math_fn f) {
  double i, result = 0, h;

  h = (b-a)/n;

  for (i = a; i < b; i += h) {
    result += f(i);
  }

  return result;
}

double g(double x) {
  return x*x*sin(x);
}

double h(double x) {
  return sqrt(4-x*x);
}
