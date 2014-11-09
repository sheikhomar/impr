#include <stdio.h>

#define SECS_PER_HOUR 3600
#define SECS_PER_MIN 60

int main(void) {
  int total_seconds;
  printf("Please type in seconds: ");
  scanf("%d", &total_seconds);

  int hours = total_seconds / SECS_PER_HOUR;
  int secs_reminder = total_seconds % SECS_PER_HOUR;
  int mins = secs_reminder / SECS_PER_MIN;
  int secs = secs_reminder % SECS_PER_MIN;
  printf("%d time, %d minutter, %d sekunder\n", hours, mins, secs);

  return 0;
}
