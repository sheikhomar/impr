#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME_LGT 50
#define MAX_PEOPLE 11

struct person{
  char fornavn[MAX_NAME_LGT];
  char efternavn[MAX_NAME_LGT];
  char vejnavn[MAX_NAME_LGT];
  int vejnummer;
  int postnummer;
  char bynavn[MAX_NAME_LGT];
};

typedef struct person Person;

void read_people_from_file(Person *people, const int people_size, const char *fileName) {
  int i;
  FILE *handle = fopen(fileName, "r");

  if (handle != NULL) {        /* File could be opened */
    for (i = 0; i < people_size; i++) {
      fscanf(handle, " %[^ ]", people[i].fornavn);
      fscanf(handle, " %[^,]", people[i].efternavn);
      fscanf(handle, ", %[^0987654321]", people[i].vejnavn);
      fscanf(handle, " %d", &people[i].vejnummer);
      fscanf(handle, ", %d", &people[i].postnummer);
      fscanf(handle, " %[^.]. ", people[i].bynavn);
    }
  }

  fclose(handle);
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

  read_people_from_file(people, MAX_PEOPLE, "people.txt");
  printf("People read from file:\n");
  print_people(people, MAX_PEOPLE);

  sort_people(people, MAX_PEOPLE);
  printf("Sorted list of people:\n");
  print_people(people, MAX_PEOPLE);

  write_people_to_file(people, MAX_PEOPLE, "people-output.tmp");

  return 0;
}
