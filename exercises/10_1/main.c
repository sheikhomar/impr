#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void strrev(char *str) {
  char *temp = (char*) malloc(strlen(str) * sizeof(char));
  int i;
  strcpy(temp, str);
  for (i = 0; i <= strlen(str); i++) {
    str[i] = temp[strlen(str)-i-1];
  }
  free(temp);
}

int main(void) {
  char input[] = "radioactivegarbage";
  strrev(input);

  printf("%s\n", input);
  return 0;
}
