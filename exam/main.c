#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

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
  int lowest_home_spectators;
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

Team *find_or_create_team(Team teams[], int *team_count, const char *team_name) {
  int i = 0, found = 0;

  for (i = 0; i < *team_count; i++) {
    if (strcmp(teams[i].name, team_name) == 0) {
      found = 1;
      break;
    }
  }

  if (!found) {
    strcpy(teams[i].name, team_name);
    teams[i].home_wins = 0;
    teams[i].away_wins = 0;
    teams[i].lowest_home_spectators = INT_MAX;
    (*team_count)++;
  }

  return &teams[i];
}

void build_team_structures(const Round rounds[], int round_count, Team teams[], int *team_count) {
  int i, j;
  Match *match;
  Team *home_team, *away_team;

  *team_count = 0;
  for (i = 0; i < round_count; i++) {
    for (j = 0; j < MAX_MATCHES_PER_ROUND; j++) {
      match = &rounds[i].matches[j];

      home_team = find_or_create_team(teams, team_count, match->home_team);
      if (match->home_goals > match->away_goals) {
        home_team->home_wins++;
      }

      if (match->spectator_count < home_team->lowest_home_spectators)
        home_team->lowest_home_spectators = match->spectator_count;

      away_team = find_or_create_team(teams, team_count, match->away_team);
      if (match->away_goals > match->home_goals) {
        away_team->away_wins++;
      }
    }
  }
}

void print_teams_with_more_away_wins(const Team teams[], int team_count) {
  int i;

  printf("Teams with more aways wins than home wins:\n");
  for (i = 0; i < team_count; i++) {
    if (teams[i].away_wins > teams[i].home_wins) {
      printf(" Team: %s %d %d\n",
          teams[i].name,
          teams[i].home_wins,
          teams[i].away_wins);
    }
  }
}


int compare_teams_by_spectator_count(const void *a, const void *b) {
  Team *team1 = (Team *)a;
  Team *team2 = (Team *)b;

  return team1->lowest_home_spectators - team2->lowest_home_spectators;
}
void print_team_with_lowest_spectator_count_at_home(Team teams[], int team_count) {
  if (team_count > 0) {
    qsort(teams, team_count, sizeof(Team), compare_teams_by_spectator_count);
    printf("%s is the team with the lowest spectator count %d at home!\n",
        teams[0].name, teams[0].lowest_home_spectators);
  }
}

void print_matches_played_on(const Round rounds[], int round_count, const char *weekday,
    const char *from, const char *to) {
  int i, j, scan_res, from_hour, from_minute, to_hour, to_minute;
  int filter_start, filter_end;
  int match_time;
  Match *match;

  scan_res = sscanf(from, " %d.%d", &from_hour, &from_minute);
  assert(scan_res == 2);
  filter_start = from_hour * 60 + from_minute;

  scan_res = sscanf(to, " %d.%d", &to_hour, &to_minute);
  assert(scan_res == 2);
  filter_end = to_hour * 60 + to_minute;

  printf("Matches played on %s from %s to %s:\n", weekday, from, to);

  for (i = 0; i < round_count; i++) {
    for (j = 0; j < MAX_MATCHES_PER_ROUND; j++) {
      match = &rounds[i].matches[j];

      if (strcmp(match->weekday, weekday) == 0) {
        match_time = match->hour * 60 + match->minute;

        if (match_time >= filter_start && match_time <= filter_end)
          print_match(match);
      }
    }
  }
}


int main(void) {
  Round rounds[MAX_ROUNDS];
  Team teams[12];
  int round_count;
  int team_count;

  load_match_results(rounds, &round_count, "superliga-2013-2014");
  compute_total_goals(rounds, round_count);
  build_team_structures(rounds, round_count, teams, &team_count);

  print_matches_goals_above_limit(rounds, round_count, 6);

  print_round_with_most_goals(rounds, round_count);

  print_teams_with_more_away_wins(teams, team_count);

  print_team_with_lowest_spectator_count_at_home(teams, team_count);

  print_matches_played_on(rounds, round_count, "Fre", "18.05", "19.05");

  return 0;
}
