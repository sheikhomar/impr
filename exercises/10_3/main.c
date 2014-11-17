#include <stdio.h>
#include <string.h>

void plurize(char* result, const char *noun) {
  int str_len = strlen(noun);
  if (noun[str_len-1] == 'y') {
    strncpy(result, noun, str_len-1);
    strcpy(result + str_len-1 , "ies");
  } else if (noun[str_len-1] == 's' || (noun[str_len-1] == 'h' && (noun[str_len-2] == 'c' || noun[str_len-2] == 's'))) { /* when noun ends with s, ch or sh */
    strcpy(result, noun);
    strcpy(result + str_len-1 , "es");
  } else {
    strcpy(result, noun);
    strcpy(result + str_len, "s");
  }
}

int main(void) {
  char *nouns[] = { "chair", "dairy", "boss", "circus", "fly", "dog", "church", "clue", "dish" };
  char result[20];
  int i;

  for (i = 0; i < 9; i++) {
    plurize(result, nouns[i]);
    printf("singular: %s plural: %s\n", nouns[i], result);
  }

  return 0;
}
