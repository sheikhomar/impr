#include <stdio.h>

#define SECS_PER_HOUR 3600
#define SECS_PER_MIN 60

int main(void) {
  int total_seconds, hours, secs_reminder, mins, secs;

  printf("Indtast antal af sekunder: ");
  scanf("%d", &total_seconds);

  hours = total_seconds / SECS_PER_HOUR;
  secs_reminder = total_seconds % SECS_PER_HOUR;
  mins = secs_reminder / SECS_PER_MIN;
  secs = secs_reminder % SECS_PER_MIN;

  printf("%d svarer til ", total_seconds);
  if (hours > 0)
    printf(hours == 1 ? "%d time, " : "%d timer, ", hours);
  if (mins > 0)
    printf(mins == 1 ? "%d minut og " : "%d minutter og ", mins);
  if (secs > 0)
    printf(secs == 1 ? "%d sekund " : "%d sekunder", secs);
  printf("\n");

  return 0;
}
