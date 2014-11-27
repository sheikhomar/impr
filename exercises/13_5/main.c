#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_NAME_LGT 50
#define MAX_PEOPLE 15
#define MAX_CHARS_PER_LINE 100

struct person{
  char fornavn[MAX_NAME_LGT];
  char efternavn[MAX_NAME_LGT];
  char vejnavn[MAX_NAME_LGT];
  int vejnummer;
  int postnummer;
  char bynavn[MAX_NAME_LGT];
};

typedef struct person Person;

void read_people_from_file(Person *people, int *people_size, const char *fileName) {
  int i = 0, scanres;
  char line[MAX_CHARS_PER_LINE];
  FILE *handle = fopen(fileName, "r");

  if (handle != NULL) {
    while (fgets(line, MAX_CHARS_PER_LINE, handle) != NULL) {
      scanres = sscanf(line, " %[^ ] %[^,], %[^0987654321] %d, %d %[^.].",
          people[i].fornavn,
          people[i].efternavn,
          people[i].vejnavn,
          &people[i].vejnummer,
          &people[i].postnummer,
          people[i].bynavn);

      assert(scanres == 6);

      i++;
    }
    *people_size = i;

    fclose(handle);
  }

}

void print_people(const Person *people, const int people_size) {
  int i;
  for (i = 0; i < people_size; i++) {
    printf(" %-10s", people[i].fornavn);
    printf(" %-10s ", people[i].efternavn);
    printf(" %-25s", people[i].vejnavn);
    printf(" %-4i", people[i].vejnummer);
    printf(" %-5i ", people[i].postnummer);
    printf(" %s \n", people[i].bynavn);
  }
}

int compare_people(const void *a, const void *b) {
  Person *person1 = (Person *)a;
  Person *person2 = (Person *)b;

  return strcmp(person1->efternavn, person2->efternavn);
}

void sort_people(Person *people, const int people_size) {
  qsort(people, people_size, sizeof(Person), compare_people);
}

void write_people_to_file(const Person *people, const int people_size, const char *fileName) {
  int i;
  FILE *handle = fopen(fileName, "w");

  for (i = 0; i < people_size; i++) {
    fprintf(handle, "%s: %s\n", people[i].bynavn, people[i].efternavn);
  }

  fclose(handle);
}

int main(void) {
  Person people[MAX_PEOPLE];
  int people_size;

  read_people_from_file(people, &people_size, "people.txt");
  printf("People read from file:\n");
  print_people(people, people_size);

  sort_people(people, people_size);
  printf("Sorted list of people:\n");
  print_people(people, people_size);

  write_people_to_file(people, people_size, "people-output.tmp");

  return 0;
}
