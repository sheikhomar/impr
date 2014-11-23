#include <stdio.h>
#include <string.h>

int is_palindrome_iter(char *str) {
  int i;
  int str_len = strlen(str);

  for (i = 0; i < str_len; i++)
    if (str[i] != str[str_len-i-1])
      return 0;

  return 1;
}

int is_palindrome_rec_ex(char *str, int str_len) {
  if (str[0] != str[str_len-1])
    return 0;

  if (str_len <= 2)
    return 1;

  return is_palindrome_rec_ex(str + 1, str_len-2);
}

int is_palindrome_rec(char *str) {
  int str_len = strlen(str);
  return is_palindrome_rec_ex(str, str_len);
}

int main(void) {
  printf("Result of iterative function: %d\n", is_palindrome_iter("regninger"));
  printf("Result of recursive function: %d\n", is_palindrome_rec("regninger"));
  return 0;
}
