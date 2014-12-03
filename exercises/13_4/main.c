#include <stdio.h>


void read_matrix_from_file(const char *fileName, double **matrix, int *m, int *n) {
  int i;
  FILE *handle = fopen(fileName, "r");

  if (handle != NULL) {        /* File could be opened */
    fscanf(handle, " %i %i", m, n);

    matrix = (double*)calloc(*m, sizeof(double));
    for (i = 0; i < *m; i++) {
      matrix[i] = (double*)calloc(*n, sizeof(double));
    }

    matrix[0][2] = 1.0;

    printf("%i %i\n", *m, *n);
  }

  fclose(handle);
}

int main(void) {
  double **matrix;
  int m, n, i, j;

  read_matrix_from_file("matrix1.txt", matrix, &m, &n);

  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      printf("%lf ", matrix[i][j]);
    }
    printf("\n");
  }


  return 0;
}
