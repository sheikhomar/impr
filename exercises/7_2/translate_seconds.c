#include "translate_seconds.h"

#define SECS_PER_HOUR 3600
#define SECS_PER_MIN 60

void translate_seconds(int total_seconds, int *hours, int *minutes, int *seconds) {
  int secs_reminder;

  *hours = total_seconds / SECS_PER_HOUR; 
  secs_reminder = total_seconds % SECS_PER_HOUR;
  *minutes = secs_reminder / SECS_PER_MIN;
  *seconds = secs_reminder % SECS_PER_MIN;
}
