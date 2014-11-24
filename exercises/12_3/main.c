#include <stdio.h>
#include <stdlib.h>

#define DATE_MAX 1000

/* Date related types */
enum weekday {sunday, monday, tuesday, wednesday, thursday,
           friday, saturday};
typedef enum weekday weekday;

struct date {
  weekday day_of_week;
  int day;
  int month;
  int year;
};

typedef struct date date;

/* Funktion prototypes */

int leapYear(int);
int date_before(date, date);
weekday next_day_of(weekday);
void prnt_date(date);
char *name_of_weekday(date);
date tomorrow(date);

int main(void) {
  date calendar[DATE_MAX];

  date first_date = {monday, 24, 11, 2014},
       last_date  = {wednesday, 17, 12, 2015},
       current_date;
  int i = 0, j = 0;

  current_date = first_date;

  while (date_before(current_date, last_date)){
    calendar[i] = current_date;
    current_date = tomorrow(current_date);
    i++;
  }

  for (j = 0; j < i; j++)
    prnt_date(calendar[j]);

  return 0;
}

/* Is date d1 less than date d2 */
int date_before(date d1, date d2){
 return
  (d1.year < d2.year) ||
  (d1.year == d2.year && d1.month < d2.month) ||
  (d1.year == d2.year && d1.month == d2.month && d1.day < d2.day);
}

/* Return the date after d */
date tomorrow(date d){
  date t = d;

  t.day_of_week = next_day_of(t.day_of_week);
  t.day = t.day + 1;

  switch (t.month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      if (t.day > 31) {
        t.day = 1;
        t.month++;
      }
      break;
    case 2:
      if (leapYear(t.year)) {
        if (t.day > 29) {
          t.day = 1;
          t.month++;
        }
      } else {
        if (t.day > 28) {
          t.day = 1;
          t.month++;
        }
      }
      break;
    default:
      if (t.day > 30) {
        t.day = 1;
        t.month++;
      }
  }

  if (t.month > 12) {
    t.month = 1;
    t.year = t.year + 1;
  }

  return t;
}

/* Is y a leapyear */
int leapYear(int y){
  int result;

  if (y % 400 == 0) result = 1;
  else if (y % 100 == 0) result = 0;
  else if (y % 4 == 0) result = 1;
  else result = 0;

  return result;
}

weekday next_day_of(weekday d){
  return (weekday) (((int) d + 1) % 7);
}

/* Print date d */
void prnt_date(date d){
  printf("%9s %2i.%2i.%4i\n", name_of_weekday(d), d.day, d.month, d.year);
}

/* Return the name of the weekday of the date d */
char *name_of_weekday(date d){
  char *result;
  switch (d.day_of_week) {
    case sunday: result = "Sunday";
       break;
    case monday: result = "Monday";
       break;
    case tuesday: result = "Tuesday";
       break;
    case wednesday: result = "Wednesday";
       break;
    case thursday: result = "Thursday";
       break;
    case friday: result = "Friday";
       break;
    case saturday: result = "Saturday";
       break;
  }
  return result;
}
