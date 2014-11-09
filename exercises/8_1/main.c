#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum first_course { guacamole, patty_shell, salmon_roll, pumpkin_soup };
enum main_course { fish, beef, gullash, forloren_hare };
enum desert { pancakes_with_ice_cream, carrot_cake, chocolate_mousse, lemon_souffle};

int random_number() {
  return rnd() % 4 + 1;
}

void meal() {

}

int main(void) {
  srand(time(NULL));

  return 0;
}
