#include <stdio.h>
#include <stdlib.h>

#define DECK_SIZE 55

/* Since I can't #define an array of chars, a readonly global array should do! 
 * Represents the suits [C]lubs, [D]iamonds, [H]earts, [S]pades, and [J]oker */
const char SUIT_CHARS[] = {'C', 'D', 'H', 'S', 'J'};

enum suit { clubs, diamonds, hearts, spades, joker };

typedef enum suit Suit;

struct card {
  Suit suit;
  int value;
};

typedef struct card Card;

void make_deck(Card deck[]);
void print_deck(Card deck[]);
void sort_deck(Card deck[]);
int compare_cards(const void *input1, const void *input2);

int main(void) {
  Card deck[DECK_SIZE];

  printf("Make deck\n");
  make_deck(deck);
  print_deck(deck);

  printf("Sort deck\n");
  sort_deck(deck);
  print_deck(deck);

  return 0;
}

void make_deck(Card deck[]) {
  int i;
  for (i = 0; i < DECK_SIZE; i++) {
    deck[i].suit = (Suit)(i/13);
    deck[i].value = i%13 + 1;
  }
}

void print_deck(Card deck[]) {
  int i;
  for (i = 0; i < DECK_SIZE; i++) {
    /* The output could be more pretty */
    printf("%3i/%c ", deck[i].value, SUIT_CHARS[deck[i].suit]);
    if (i % 13 == 12) 
      printf("\n");
  }

  printf("\n\n");
}

void sort_deck(Card deck[]) {
  qsort(deck, DECK_SIZE, sizeof(Card), compare_cards);
}

int compare_cards(const void *input1, const void *input2) {
  Card *card1 = (Card*)input1;
  Card *card2 = (Card*)input2;

  /* Make sure that aces have higher values than kings */
  int val1 = card1->value == 1 ? 14 : card1->value;
  int val2 = card2->value == 1 ? 14 : card2->value;

  /* Compare suits of the two cards by substraction instead of if-conditions */
  int suit_diff = card1->suit - card2->suit;
  
  /* Compare values if the suits are the same */
  if (suit_diff == 0)
    return val1 - val2;

  return suit_diff;
}
