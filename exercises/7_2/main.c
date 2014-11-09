#include <stdio.h>
#include "translate_seconds.h"

int main(void) {
  
  int totalSeconds = 3600, hours, minutes, seconds;


  translate_seconds(totalSeconds, &hours, &minutes, &seconds);

  return 0;
}
