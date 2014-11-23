#include <stdio.h>
#include <string.h>

/* Longest common suffix */
void lcs(char *result, const char *word1, const char *word2) {
  int word1_len = strlen(word1);
  int word2_len = strlen(word2);
  int i = 0;

  while(word1[word1_len-i] == word2[word2_len-i]) {
    i++;
  }
  i--;
  strcpy(result, word1+(word1_len-i));
}

int main(void) {
  char result[20];

  lcs(result, "hestevaedeloeb", "hundeslaedevaedeloeb");
  printf("%s\n", result);

  lcs(result, "diagram", "program");
  printf("%s\n", result);

  return 0;
}
