#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Assume each line in the provided file to contain 58 characters */
#define MAX_CHARS_PER_LINE 60

/* Assume that there can only be six matches per round */
#define MAX_MATCHES_PER_ROUND 6
#define MAX_ROUNDS 33

struct match {
  char weekday[5];
  int day;
  int month;
  int hour;
  int minute;
  char home_team[4];
  char away_team[4];
  int home_goals;
  int away_goals;
  int spectator_count;
};

typedef struct match Match;

struct round {
  unsigned char number;
  Match matches[MAX_MATCHES_PER_ROUND];
};

typedef struct round Round;

int is_new_line(char *string) {
  return strcmp(string, "\r\n") == 0;
}

void print_match(const Match *match) {
  printf("%s %02d/%02d %02d:%02d  %3s vs. %3s [%d:%d] (%6d)\n",
      match->weekday,
      match->day,
      match->month,
      match->hour,
      match->minute,
      match->home_team,
      match->away_team,
      match->home_goals,
      match->away_goals,
      match->spectator_count
      );
}

void parse_match(Match *match, const char *line) {
  int scan_res;
  double spectator_count;

  scan_res = sscanf(line, " %s %d/%d %d.%d %s - %s %d - %d %lf",
       match->weekday,
      &match->day,
      &match->month,
      &match->hour,
      &match->minute,
       match->home_team,
       match->away_team,
      &match->home_goals,
      &match->away_goals,
      &spectator_count
      );

  /* FIXME: Ugly hack. */
  if (spectator_count < 1000)
    spectator_count *= 1000;
  match->spectator_count = spectator_count;

  assert(scan_res == 10);
}

void load_match_results(Round *rounds, int *round_count, const char *file_name) {
  int round_index = 0, match_index = 0;
  char line[MAX_CHARS_PER_LINE];
  FILE *handle;
  Match *match;

  *round_count = 0;
  handle = fopen(file_name, "r");
  if (handle != NULL) {
    while (fgets(line, MAX_CHARS_PER_LINE, handle) != NULL) {
      if (is_new_line(line)) {
        match_index = 0;
        round_index++;
        (*round_count)++;
      } else {
        match = &(rounds[round_index].matches[match_index]);
        parse_match(match, line);
        match_index++;
        if (*round_count == 0)
          *round_count = 1;
      }
    }
    fclose(handle);
  } else {
    printf("ERROR: File %s cannot be opened.\n", file_name);
    exit(-1);
  }
}

void print_matches_goals_above_limit(const Round *rounds, int round_count, int limit) {
  int i, j, total_goals;
  Match *match;

  for (i = 0; i < round_count; i++) {
    for (j = 0; j < MAX_MATCHES_PER_ROUND; j++) {
      match = &rounds[i].matches[j];
      total_goals = match->home_goals + match->away_goals;
      if (total_goals > limit)
        print_match(match);
    }
  }
}

int main(void) {
  Round rounds[MAX_ROUNDS];
  int round_count;

  load_match_results(rounds, &round_count, "superliga-2013-2014");
  printf("\nRounds %d \n", round_count);

  print_matches_goals_above_limit(rounds, round_count, 6);

  return 0;
}
