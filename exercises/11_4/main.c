#include <stdio.h>
#define MAX_X 5
#define MAX_Y 5

int blob_count(int x, int y, int grid[MAX_X][MAX_Y]) {
  if (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) {
    if (grid[x][y] == 1) {

      grid[x][y] = 0;
      return 1 +
        blob_count(x-1, y-1, grid) +
        blob_count(x-1, y-0, grid) +
        blob_count(x-1, y+1, grid) +
        blob_count(x-0, y-1, grid) +
        blob_count(x-0, y+1, grid) +
        blob_count(x+1, y-1, grid) +
        blob_count(x+1, y-0, grid) +
        blob_count(x+1, y+1, grid);
    }
  } 

  return 0;
}

int main(void) {
  int grid[MAX_X][MAX_Y] = {
                     {1, 1, 0, 0, 0},
                     {0, 1, 1, 0, 0},
                     {0, 0, 1, 1, 1},
                     {1, 0, 0, 0, 1},
                     {0, 1, 0, 1, 1},
                    };

  printf("Blob count: %d\n", blob_count(1, 1, grid));
  return 0;
}
