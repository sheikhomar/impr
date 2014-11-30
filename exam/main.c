#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define MAX_CHARS_PER_LINE 60
#define MAX_MATCHES_PER_ROUND 6
#define MAX_ROUNDS 33
#define MAX_TEAMS 12

struct line_info {
  char weekday[4];
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

typedef struct line_info LineInfo;
struct round {
  int number;
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

struct match {
  char weekday[5];
  int day;
  int month;
  int hour;
  int minute;
  int home_goals;
  int away_goals;
  int spectator_count;
  int total_goals;
  Team *away_team;
  Team *home_team;
  int round;
};

typedef struct match Match;

struct tournament {
  Match matches[MAX_MATCHES_PER_ROUND * MAX_ROUNDS];
  int match_count;

  Team teams[MAX_TEAMS];
  int team_count;

  Round rounds[MAX_ROUNDS];
  int round_count;
};

typedef struct tournament Tournament;

int is_empty(char *line) {
  return strcmp(line, "\r\n") == 0;
}

void print_match(const Match *match) {
  printf(" (Round %02d) %s %02d/%02d %02d:%02d  %3s vs. %3s [%d:%d|%d] (%6d)\n",
      match->round,
      match->weekday,
      match->day,
      match->month,
      match->hour,
      match->minute,
      match->home_team->name,
      match->away_team->name,
      match->home_goals,
      match->away_goals,
      match->total_goals,
      match->spectator_count
      );
}

void print_matches_by_goals_scored(const Match matches[], int match_count, int goals) {
  int i;

  printf("Matches with %d or more goals:\n", goals);
  for (i = 0; i < match_count; i++)
    if (matches[i].total_goals >= goals)
      print_match(&matches[i]);
}

int compare_rounds_by_total_goals(const void *a, const void *b) {
  Round *round1 = (Round *)a;
  Round *round2 = (Round *)b;
  int goals_diff = round2->total_goals - round1->total_goals;

  if (goals_diff == 0)
    return round1->number - round2->number;

  return goals_diff;
}

void print_round_with_most_goals(Round rounds[], int round_count) {
  if (round_count > 0) {
    qsort(rounds, round_count, sizeof(Round), compare_rounds_by_total_goals);
    printf("Most goals scored in round #%d with %d goals\n",
        rounds[0].number, rounds[0].total_goals);
  }
}

Team *find_or_create_team2(Team teams[], int *team_count, const char *team_name) {
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

void print_teams_with_more_away_wins(const Team teams[], int team_count) {
  int i;

  printf("Teams with more aways wins than home wins:\n");
  for (i = 0; i < team_count; i++) {
    if (teams[i].away_wins > teams[i].home_wins) {
      printf(" - %s has %d away wins and %d home wins \n",
          teams[i].name,
          teams[i].away_wins,
          teams[i].home_wins);
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

/*
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
*/

int convert_to_int(const char input[]) {
  double return_value = 0.0;

  sscanf(input, "%lf", &return_value);

  /* FIXME: Ugly hack. Research how to do this properly! */
  return (int)(return_value * 1000);
}

void parse_line(LineInfo *line_info, const char line[]) {
  int scan_res;
  char str_spectator_count[10];

  scan_res = sscanf(line, " %s %d/%d %d.%d %s - %s %d - %d %[^ ]",
       line_info->weekday,
      &line_info->day,
      &line_info->month,
      &line_info->hour,
      &line_info->minute,
       line_info->home_team,
       line_info->away_team,
      &line_info->home_goals,
      &line_info->away_goals,
       str_spectator_count
      );

  /* Fail fast if line format is incorrect */
  assert(scan_res == 10);

  line_info->spectator_count = convert_to_int(str_spectator_count);
}

void initialize_team(Team *team, const char team_name[]) {
  strcpy(team->name, team_name);
  team->home_wins = 0;
  team->away_wins = 0;
  team->lowest_home_spectators = INT_MAX;
}

/* Finds a team in a tournament by name. If a team with the given team_name
 * does not exist, a new team is created in the tournament */
Team *find_or_create_team(Tournament *tournament, const char team_name[]) {
  int i = 0;
  Team *team = NULL;

  /* Search for a team in the tournament */
  for (i = 0; i < tournament->team_count; i++) {
    if (strcmp(tournament->teams[i].name, team_name) == 0) {
      team = &tournament->teams[i];
      break;
    }
  }

  if (!team) {
    /* A new team is created by simply incrementing team_count variable
     * in the tournament instance. */
    tournament->team_count++;

    /* Fail fast if amount of teams exceeded reserved memory */
    assert(tournament->team_count <= MAX_TEAMS);

    team = &tournament->teams[tournament->team_count - 1];

    initialize_team(team, team_name);
  }

  return team;
}

void initialize_match(Match *match, LineInfo *line_info) {
  strcpy(match->weekday, line_info->weekday);

  match->day = line_info->day;
  match->month = line_info->month;
  match->hour = line_info->hour;
  match->minute = line_info->minute;
  match->home_goals = line_info->home_goals;
  match->away_goals = line_info->away_goals;
  match->spectator_count = line_info->spectator_count;
  match->total_goals = match->home_goals + match->away_goals;
}

/* Create a match in a tournament */
Match *build_match(Tournament *tournament, int round, LineInfo *line_info) {
  Match *match;

  /* A new match is created by incrementing counter variable
   * in the tournament instance. */
  tournament->match_count++;

  /* Fail fast if amount of teams exceeded reserved memory */
  assert(tournament->match_count <= MAX_MATCHES_PER_ROUND * MAX_ROUNDS);

  match = &tournament->matches[tournament->match_count - 1];

  initialize_match(match, line_info);

  match->home_team = find_or_create_team(tournament, line_info->home_team);
  match->away_team = find_or_create_team(tournament, line_info->away_team);
  match->round = round;

  return match;
}

void reset_round(Round *round) {
  round->number = 0;
  round->total_goals = 0;
}

/* Build a round of matches in a tournament given lines of matches */
void build_rounds(Tournament *tournament, char match_lines[][MAX_CHARS_PER_LINE], int line_count) {
  int i = 0;
  LineInfo line_info;
  Match *match;
  Round *round;

  /* A new round is created by incrementing counter variable
   * in the tournament instance. */
  tournament->round_count++;

  /* Fail fast if amount of rounds exceeds reserved memory */
  assert(tournament->round_count <= MAX_ROUNDS);

  /* Fetch the next object */
  round = &tournament->rounds[tournament->round_count - 1];

  reset_round(round);

  round->number = tournament->round_count;

  for (i = 0; i < line_count; i++) {
    parse_line(&line_info, match_lines[i]);
    match = build_match(tournament, tournament->round_count, &line_info);
    round->total_goals += match->total_goals;
  }
}

/* Resets a Tournament instance */
void reset_tournament(Tournament *tournament) {
  tournament->match_count = 0;
  tournament->team_count = 0;
  tournament->round_count = 0;
}

void update_team_stats(Tournament *tournament) {
  int i;
  Match *match;
  Team *home_team, *away_team;

  for (i = 0; i < tournament->match_count; i++) {
    match = &tournament->matches[i];
    home_team = match->home_team;
    away_team = match->away_team;

    if (match->home_goals > match->away_goals)
      home_team->home_wins++;

    if (match->spectator_count < home_team->lowest_home_spectators)
      home_team->lowest_home_spectators = match->spectator_count;

    if (match->away_goals > match->home_goals)
      away_team->away_wins++;
  }
}

/* Builds a Tournament instance using the given file */
void build_tournament(Tournament *tournament, char file_name[]) {
  FILE *handle;
  char line[MAX_CHARS_PER_LINE];
  char match_lines[MAX_MATCHES_PER_ROUND][MAX_CHARS_PER_LINE];
  int line_index = 0;

  handle = fopen(file_name, "r");

  if (handle != NULL) {
    reset_tournament(tournament);

    while (fgets(line, MAX_CHARS_PER_LINE, handle) != NULL) {
      /*printf("%d: %s", line_index, line);*/
      if (is_empty(line)) {
        /* Delegate the actual parsing to another function */
        build_rounds(tournament, match_lines, line_index);

        /* Restart the line index */
        line_index = 0;
      } else {
        /* Fail fast if input file contains more matches
         * per round than reserved memory in the program */
        assert(MAX_MATCHES_PER_ROUND >= line_index + 1);
        strcpy(match_lines[line_index], line);
        line_index++;
      }
    }

    fclose(handle);

    if (line_index > 0) {
      build_rounds(tournament, match_lines, line_index);
    }

    update_team_stats(tournament);
  } else {
    printf("ERROR: File '%s' cannot be opened.\n", file_name);
    exit(-1);
  }
}


int main(void) {
  Tournament tournament;

  build_tournament(&tournament, "superliga-2013-2014");

  print_matches_by_goals_scored(tournament.matches, tournament.match_count, 7);
  print_round_with_most_goals(tournament.rounds, tournament.round_count);
  print_teams_with_more_away_wins(tournament.teams, tournament.team_count);
  print_team_with_lowest_spectator_count_at_home(tournament.teams, tournament.team_count);
  /*
  print_matches_played_on(rounds, round_count, "Fre", "18.05", "19.05");
  */

  return 0;
}
