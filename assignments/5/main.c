#include <stdio.h>

void dispense_cash(int desiredAmount, int* notes100, int* notes50, int* notes20, int* notes10) {
  int reminder;

  *notes100 = desiredAmount / 100;
  reminder = desiredAmount % 100;

  *notes50 = reminder / 50;
  reminder %= 50;

  *notes20 = reminder / 20;
  reminder %= 20;

  *notes10 = reminder / 10;
}

int main(void) {
  int amount, notes100, notes50, notes20, notes10;

  do {
    printf("Please enter amount divisible by 10: ");
    scanf("%d", &amount);
    printf("\n");
  } while (amount % 10 != 0);

  dispense_cash(amount, &notes100, &notes50, &notes20, &notes10);

  printf("ATM machine should dispense following amount of notes:\n");
  printf("100: %d\n", notes100);
  printf("50:  %d\n", notes50);
  printf("20:  %d\n", notes20);
  printf("10:  %d\n", notes10);

  return 0;
}
