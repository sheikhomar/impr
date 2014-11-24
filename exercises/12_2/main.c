#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 10

struct book {
  char *title, *author, *publisher;
  int publishing_year;
  int university_text_book;
};

typedef struct book book;

book make_book(char *title, char *author, char *publisher,
               int year, int text_book){
  book result;                   /* Consider allocation of memory to string fields. */
  result.title = title; result.author = author;
  result.publisher = publisher; result.publishing_year = year;
  result.university_text_book = text_book;

  return result;
}

void prnt_book(book b){
  char *yes_or_no;

  yes_or_no = (b.university_text_book ? "yes" : "no");
  printf("Title: %s\n"
         "Author: %s\n"
         "Publisher: %s\n"
         "Year: %4i\n"
         "University text book: %s\n\n",
         b.title, b.author, b.publisher,
         b.publishing_year, yes_or_no);
}

int compare_books_by_title(const void *input1, const void *input2) {
  book *book1 = (book*)input1;
  book *book2 = (book*)input2;

  return strcmp(book1->title, book2->title);
}

int compare_books_by_kind_and_year(const void *input1, const void *input2) {
  book *book1 = (book*)input1;
  book *book2 = (book*)input2;

  if (book1->university_text_book == book1->university_text_book)
    return book1->publishing_year - book2->publishing_year;
  return book1->university_text_book - book1->university_text_book;
}

void sort_books_by_title(book shelf[], int size){
  qsort(shelf, size, sizeof(book), compare_books_by_title);
}

void sort_books_by_kind_and_year(book shelf[], int size) {
  qsort(shelf, size, sizeof(book), compare_books_by_kind_and_year);
}

void print_shelf(book shelf[], int size) {
  int i;
  for(i = 0; i < size; i++)
    prnt_book(shelf[i]);
}

int main(void) {
  book shelf[MAX_BOOKS];

  shelf[0] =
    make_book("Problem Solving and Program Design in C", "Hanly and Koffman",
              "Pearson", 2010, 1);

  shelf[1] =
    make_book("C by Disssection", "Kelley and Pohl",
              "Addison Wesley", 2001, 1);

  shelf[2] =
    make_book("The C Programming Language", "Kernighan og Ritchie",
              "Prentice Hall", 1988, 1);

  shelf[3] =
    make_book("Pelle Erobreren", "Martin Andersen Nexoe",
              "Gyldendal", 1910, 0);

  shelf[4] = shelf[3];
  shelf[4].title = "Ditte Menneskebarn";
  shelf[4].publishing_year = 1917;

  print_shelf(shelf, 5);

  printf("=== Books sorted by title \n\n");
  sort_books_by_title(shelf, 5);
  print_shelf(shelf, 5);

  printf("=== Books sorted by year \n\n");
  sort_books_by_kind_and_year(shelf, 5);
  print_shelf(shelf, 5);


  return 0;
}
