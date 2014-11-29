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
  int total_goals;
};

typedef struct match Match;

struct round {
  int number;
  Match matches[MAX_MATCHES_PER_ROUND];
  int total_goals;
};

typedef struct round Round;

struct team {
  char name[4];
  int home_wins;
  int away_wins;
};

typedef struct team Team;

int is_empty(char *line) {
  return strcmp(line, "\r\n") == 0;
}

void print_match(const Match *match) {
  printf(" %s %02d/%02d %02d:%02d  %3s vs. %3s [%d:%d] (%6d)\n",
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
      if (is_empty(line)) {
        match_index = 0;
        round_index++;
        (*round_count)++;
      } else {
        match = &(rounds[round_index].matches[match_index]);
        parse_match(match, line);
        match_index++;
        if (*round_count == 0)
          *round_count = 1;

        rounds[round_index].number = *round_count;
      }
    }
    fclose(handle);
  } else {
    printf("ERROR: File %s cannot be opened.\n", file_name);
    exit(-1);
  }
}

void compute_total_goals(Round *rounds, int round_count) {
  int i, j, total_goals;
  Match *match;

  for (i = 0; i < round_count; i++) {
    rounds[i].total_goals = 0;
    for (j = 0; j < MAX_MATCHES_PER_ROUND; j++) {
      match = &rounds[i].matches[j];
      total_goals = match->home_goals + match->away_goals;
      match->total_goals = total_goals;
      rounds[i].total_goals += total_goals;
    }
  }
}

void print_matches_goals_above_limit(const Round *rounds, int round_count, int limit) {
  int i, j;
  Match *match;

  for (i = 0; i < round_count; i++) {
    for (j = 0; j < MAX_MATCHES_PER_ROUND; j++) {
      match = &rounds[i].matches[j];
      if (match->total_goals > limit)
        print_match(match);
    }
  }
}

int compare_rounds_by_total_goals(const void *a, const void *b) {
  Round *round1 = (Round *)a;
  Round *round2 = (Round *)b;
  int goals_diff = round2->total_goals - round1->total_goals;

  if (goals_diff == 0)
    return round1->number - round2->number;

  return goals_diff;
}

void print_round_with_most_goals(Round *rounds, int round_count) {
  if (round_count > 0) {
    qsort(rounds, round_count, sizeof(Round), compare_rounds_by_total_goals);
    printf("Most goals scored in round #%d with %d goals\n",
        rounds[0].number, rounds[0].total_goals);
  }
}

int main(void) {
  Round rounds[MAX_ROUNDS];
  int round_count;

  load_match_results(rounds, &round_count, "superliga-2013-2014");
  compute_total_goals(rounds, round_count);

  print_matches_goals_above_limit(rounds, round_count, 6);

  print_round_with_most_goals(rounds, round_count);

  return 0;
}
