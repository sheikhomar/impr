/**
 * Name:  Omar Ali Sheikh
 * Group: A400a, SW
 * Email: osheik13@student.aau.dk
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

/**
 * These constants are based on assumptions made for the input file.
 **/
#define MAX_CHARS_PER_LINE 60
#define MAX_MATCHES_PER_ROUND 6
#define MAX_ROUNDS 33
#define MAX_TEAMS 12

/** Represents fields of a non-blank line in the input file.
 *
 * Line parsing logic is encapsulated in a separate function.
 * This structure makes it easy to return parsed values as a
 * single variable.
 **/
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

  /* String version of spectator_count. The sole function of
   * str_spectator_count is to facilitate number conversion. */
  char str_spectator_count[10];
};
typedef struct line_info LineInfo;

/** Represents a round in a tournament.
 *
 * This structure makes it easy to solve problems like
 * sorting rounds by goals scored in the round. The structure
 * could also be extended to keep track of matches played
 * in a given round.
 **/
struct round {
  int number;
  int total_goals;
};
typedef struct round Round;

/** Represents a team in a tournament.
 **/
struct team {
  char name[4];
  int home_wins;
  int away_wins;
  int lowest_home_spectators;
  int won_matches;
  int tied_matches;
  int lost_matches;
  int goals_scored;
  int goals_against;
  int goals_diff;
  int points;
};
typedef struct team Team;

/** Represents a match in a tournament.
 **/
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
  int round;
  Team *away_team;
  Team *home_team;
};
typedef struct match Match;

/** Represents a tournament.
 *
 * This structure makes use of statically allocated arrays in stead of
 * dynamically allocated arrays.
 * The design rationale for this decision are as follows:
 *  1) Input file is relative small.
 *  2) Memory usage is not an issue.
 **/
struct tournament {
  Match matches[MAX_MATCHES_PER_ROUND * MAX_ROUNDS];
  int match_count;
  Team teams[MAX_TEAMS];
  int team_count;
  Round *rounds[MAX_ROUNDS];
  int round_count;
};
typedef struct tournament Tournament;

/** Determines whether the given line marks the end of a round in the input file.
 **/
int is_end_marker_for_round(char *line) {
  return strcmp(line, "\r\n") == 0;
}

/** Prints match details to the standard output.
 **/
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

/** Prints a list of matches filtered by goals.
 **/
void print_matches_by_goals_scored(const Match matches[], int match_count, int goals) {
  int i;

  printf("\n\n1) Matches with %d or more goals:\n\n", goals);
  for (i = 0; i < match_count; i++)
    if (matches[i].total_goals >= goals)
      print_match(&matches[i]);
}

int compare_rounds_by_total_goals(const void *a, const void *b) {
  Round *round1 = *((Round **)a);
  Round *round2 = *((Round **)b);

  /* Compares using substraction instead of if-conditions.
   * First sort by goals in descending order... */
  int goals_diff = round2->total_goals - round1->total_goals;

  /* ... then sort by number in ascending order */
  if (goals_diff == 0)
    return round1->number - round2->number;

  return goals_diff;
}


/** Prints the round with the highest goal score.
 **/
void print_round_with_highest_goal_score(Round *rounds[], int round_count) {
  if (round_count > 0) {
    /* Sort rounds by array in descending order */
    qsort(rounds, round_count, sizeof(Round *), compare_rounds_by_total_goals);

    printf("\n\n2) Round %d has the highest goal score with %d goals\n",
        rounds[0]->number, rounds[0]->total_goals);
  }
}

/** Prints a list of teams that have more away wins than home wins.
 * */
void print_teams_with_more_away_wins(const Team teams[], int team_count) {
  int i;

  printf("\n\n3) Teams with more aways wins than home wins:\n");
  for (i = 0; i < team_count; i++) {
    if (teams[i].away_wins > teams[i].home_wins) {
      printf(" - %s has %d away wins and %d home wins \n",
          teams[i].name,
          teams[i].away_wins,
          teams[i].home_wins);
    }
  }
}

/**
 * Compare two teams by lowest_home_spectators.
 **/
int compare_teams_by_spectator_count(const void *a, const void *b) {
  Team *team1 = (Team *)a;
  Team *team2 = (Team *)b;

  return team1->lowest_home_spectators - team2->lowest_home_spectators;
}

void print_team_with_lowest_spectator_count_at_home(Team teams[], int team_count) {
  if (team_count > 0) {
    qsort(teams, team_count, sizeof(Team), compare_teams_by_spectator_count);
    printf("\n\n4) The team with lowest spectator count at home is %s with only %d spectators!\n",
        teams[0].name, teams[0].lowest_home_spectators);
  }
}

/** Parses a string that contains time in the format 'HH.MM'.
 **/
void parse_time(const char formatted_time[], int *hours, int *minutes) {
  int scan_res;

  scan_res = sscanf(formatted_time, " %d.%d", hours, minutes);

  /* Make sure our assumptions are correct. */
  assert(scan_res == 2);
}

int get_minutes_since_midnight(int hours, int minutes) {
  return hours * 60 + minutes;
}

/** Parses a string that contains time in the format 'HH.MM' and
 * converts it to minutes since midnight.
 **/
int parse_time_as_minutes_since_midnight(const char formatted_time[]) {
  int hours, minutes;

  parse_time(formatted_time, &hours, &minutes);

  return get_minutes_since_midnight(hours, minutes);
}

/** Compares two matches by goals.
 **/
int compare_matches_by_total_goals(const void *a, const void *b) {
  Match *match1 = (Match *)a;
  Match *match2 = (Match *)b;

  return match2->total_goals - match1->total_goals;
}

/** Prints a sorted list of matches that are played on a certain weekday
 *  and within a certain time period.
 */
void print_matches_by_weekday(Match matches[], int match_count, const char *weekday,
    const char start_time[], const char end_time[]) {

  int i, filter_start_time, filter_end_time, match_time;
  Match *match;

  /* Setup time filters */
  filter_start_time = parse_time_as_minutes_since_midnight(start_time);
  filter_end_time = parse_time_as_minutes_since_midnight(end_time);

  /* Sort the list of matches by total goals in descending order */
  qsort(matches, match_count, sizeof(Match), compare_matches_by_total_goals);

  printf("\n\n5) Matches played on %s from %s to %s:\n\n", weekday, start_time, end_time);

  for (i = 0; i < match_count; i++) {
    match = &matches[i];

    if (strcmp(match->weekday, weekday) == 0) {
      match_time = get_minutes_since_midnight(match->hour, match->minute);
      if (match_time >= filter_start_time && match_time <= filter_end_time)
        print_match(match);
    }
  }
}

/** Compares two teams according to the specs.
 **/
int compare_teams_by_points(const void *a, const void *b) {
  Team *team1 = (Team *)a;
  Team *team2 = (Team *)b;
  int points_diff = team2->points - team1->points;
  int goals_diff = team2->goals_diff - team1->goals_diff;
  int goals_scored_diff = team2->goals_scored - team1->goals_scored;

  /* Priority 1: Compare by goal difference when points are equal */
  if (points_diff == 0) {
    /* Priority 2: Compare by total goals scored when goal differences are equal */
    if (goals_diff == 0) {
      /* Priority 3: Compare by name when goals scored are equal */
      if (goals_scored_diff == 0)
        return strcmp(team2->name, team1->name);
      else
        return goals_scored_diff;
    } else {
      return goals_diff;
    }
  }

  return points_diff;
}

/** Prints points table for alle teams.
 */
void print_points_table(Team teams[], int team_count) {
  int i;
  Team *team;

  /* Sort teams by points in descending order */
  qsort(teams, team_count, sizeof(Team), compare_teams_by_points);

  /* Print out the score */
  printf("\n\n6) Team points table:\n\n");
  printf(" No  Team   W    T    L   G+    G-    +/-    P\n");
  printf("-----------------------------------------------\n");
  for (i = 0; i < team_count; i++) {
    team = &teams[i];
    printf("%3d  %-3s   %2d   %2d   %2d   %2d    %2d    %+3d   %2d\n",
        i+1,
        team->name,
        team->won_matches,
        team->tied_matches,
        team->lost_matches,
        team->goals_scored,
        team->goals_against,
        team->goals_diff,
        team->points
        );
  }
}

/**
 * Prints out all solutions with default input.
 */
void print_all(Tournament *tournament) {
  printf("Running program with --print argument.");
  print_matches_by_goals_scored(tournament->matches, tournament->match_count, 7);
  print_round_with_highest_goal_score(tournament->rounds, tournament->round_count);
  print_teams_with_more_away_wins(tournament->teams, tournament->team_count);
  print_team_with_lowest_spectator_count_at_home(tournament->teams, tournament->team_count);
  print_matches_by_weekday(tournament->matches, tournament->match_count, "Fre", "18.05", "19.05");
  print_points_table(tournament->teams, tournament->team_count);
}

/** Converts a formatted string of an integer to integer.
 **/
int convert_to_int(const char input[]) {
  double return_value = 0.0;

  sscanf(input, "%lf", &return_value);

  /* FIXME: Pretty ugly! */
  return return_value * 1000;
}

/** Parses the given line of text and stores values into
 * a LineInfo structure.
 **/
LineInfo *parse_line(const char line[]) {
  /* Make use of the static storage class to avoid allocating
   * memory each time this function is called. */
  static LineInfo line_info;
  int scan_res;

  scan_res = sscanf(line, " %s %d/%d %d.%d %s - %s %d - %d %[^ ]",
       line_info.weekday,
      &line_info.day,
      &line_info.month,
      &line_info.hour,
      &line_info.minute,
       line_info.home_team,
       line_info.away_team,
      &line_info.home_goals,
      &line_info.away_goals,
       line_info.str_spectator_count
      );

  /* Make sure line format is as expected. */
  assert(scan_res == 10);

  line_info.spectator_count = convert_to_int(line_info.str_spectator_count);

  /* It is not a problem to return the reference of a local variable
   * because it has the static storage class, which means that
   * the local variable is preserved upon function completion. */
  return &line_info;
}

/** Initializes a Team structure with the given name.
 **/
void initialize_team(Team *team, const char team_name[]) {
  strcpy(team->name, team_name);
  team->home_wins = 0;
  team->away_wins = 0;
  team->lowest_home_spectators = INT_MAX;
  team->won_matches = 0;
  team->tied_matches = 0;
  team->lost_matches = 0;
  team->goals_scored = 0;
  team->goals_against = 0;
  team->goals_diff = 0;
  team->points = 0;
}

/** Creates a new team in the tournament.
 **/
Team *create_team(Tournament *tournament, const char team_name[]) {
  Team *team = NULL;

  /* A new team is created by simply incrementing team_count variable
   * in the tournament instance. */
  tournament->team_count++;

  /* Fail fast if amount of teams exceeded reserved memory */
  assert(tournament->team_count <= MAX_TEAMS);

  team = &tournament->teams[tournament->team_count - 1];

  initialize_team(team, team_name);

  return team;
}

/** Finds a team in the tournament. If the team doesn't exist
 * NULL is returned.
 **/
Team *find_team_by_name(Tournament *tournament, const char team_name[]) {
  int i = 0;

  for (i = 0; i < tournament->team_count; i++) {
    if (strcmp(tournament->teams[i].name, team_name) == 0) {
      return &tournament->teams[i];
    }
  }

  return NULL;
}

/** Finds a team in the tournament by name. If the team is
 *  not found, a new team is created in the tournament.
 **/
Team *find_or_create_team(Tournament *tournament, const char team_name[]) {
  Team *team = NULL;

  team = find_team_by_name(tournament, team_name);

  if (!team) {
    team = create_team(tournament, team_name);
  }

  return team;
}

/** Initializes a Match structure using information from
 *  LineInfo structure.
 **/
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

/** Creates a match in the tournament.
 **/
Match *create_match(Tournament *tournament, int round, LineInfo *line_info) {
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

/** Initializes a Round structure with default values.
 **/
void initialize_round(Round *round, int round_number) {
  round->number = round_number;
  round->total_goals = 0;
}

/** Creates a new round in the tournament.
 **/
Round *create_round(Tournament *tournament) {
  Round *round;

  /* Make sure a new round can be created. */
  assert(tournament->round_count + 1 <= MAX_ROUNDS);

  round = (Round *)malloc(sizeof(Round));

  if (round == NULL) {
    printf("Error in create_round(): Could not allocate memory!");
    exit(EXIT_FAILURE);
  }

  tournament->rounds[tournament->round_count] = round;
  tournament->round_count++;

  initialize_round(round, tournament->round_count + 1);

  return round;
}

/** Builds a round of matches in the tournament given lines of matches.
 */
void build_round_matches(Tournament *tournament, char match_lines[][MAX_CHARS_PER_LINE], int line_count) {
  int i = 0;
  LineInfo *line_info;
  Match *match;
  Round *round;

  round = create_round(tournament);

  for (i = 0; i < line_count; i++) {
    line_info = parse_line(match_lines[i]);
    match = create_match(tournament, tournament->round_count, line_info);
    round->total_goals += match->total_goals;
  }
}

/** Updates match statistics.
**/
void update_match_stats_for_teams(Match matches[], int match_count) {
  int i;
  Match *match;
  Team *home_team, *away_team;

  for (i = 0; i < match_count; i++) {
    /* Set convenience variables */
    match = &matches[i];
    home_team = match->home_team;
    away_team = match->away_team;

    /* Update match stats */
    if (match->home_goals > match->away_goals) {
      home_team->home_wins++;
      home_team->won_matches++;
      away_team->lost_matches++;
    } else if (match->away_goals > match->home_goals) {
      away_team->away_wins++;
      away_team->won_matches++;
      home_team->lost_matches++;
    } else {
      home_team->tied_matches++;
      away_team->tied_matches++;
    }

    /* Update goal stats */
    home_team->goals_scored += match->home_goals;
    home_team->goals_against += match->away_goals;
    away_team->goals_scored += match->away_goals;
    away_team->goals_against += match->home_goals;

    /* Update spectator stats */
    if (match->spectator_count < home_team->lowest_home_spectators)
      home_team->lowest_home_spectators = match->spectator_count;
  }
}

/** Computes goals and points for each team.
 **/
void compute_team_stats(Team teams[], int team_count) {
  int i;
  Team *team;

  for (i = 0; i < team_count; i++) {
    team = &teams[i];

    team->goals_diff = team->goals_scored - team->goals_against;
    team->points = team->won_matches * 3 + team->tied_matches;
  }
}

/** Updates tournament statistics.
 **/
void update_tournament_stats(Tournament *tournament) {
  update_match_stats_for_teams(tournament->matches, tournament->match_count);
  compute_team_stats(tournament->teams, tournament->team_count);
}

/** Initializes a Tournament structure with default values.
 **/
void initialize_tournament(Tournament *tournament) {
  tournament->match_count = 0;
  tournament->team_count = 0;
  tournament->round_count = 0;
}

/** Creates a new tournament.
 */
Tournament *create_tournament() {
  Tournament *tournament = NULL;

  tournament = (Tournament *)malloc(sizeof(Tournament));

  if (tournament == NULL) {
    printf("Error in create_tournament(): Could not allocate memory for a tournament!");
    exit(EXIT_FAILURE);
  }

  initialize_tournament(tournament);

  return tournament;
}


/** Build a new tournament using data from file.
 **/
Tournament *build_tournament(char file_name[]) {
  FILE *handle;
  char line[MAX_CHARS_PER_LINE];
  char match_lines[MAX_MATCHES_PER_ROUND][MAX_CHARS_PER_LINE];
  int line_index = 0;
  Tournament *tournament = NULL;

  handle = fopen(file_name, "r");

  if (handle != NULL) {
    tournament = create_tournament();

    while (fgets(line, MAX_CHARS_PER_LINE, handle) != NULL) {
      if (is_end_marker_for_round(line)) {
        /* Delegate the actual parsing to another function */
        build_round_matches(tournament, match_lines, line_index);
        line_index = 0;
      } else {
        /* Fail fast if input file contains more matches
         * per round than reserved memory in the program */
        assert(line_index < MAX_MATCHES_PER_ROUND);
        strcpy(match_lines[line_index], line);
        line_index++;
      }
    }

    fclose(handle);

    if (line_index > 0)
      build_round_matches(tournament, match_lines, line_index);

    /* As the final step update statistics */
    update_tournament_stats(tournament);
  } else {
    printf("Error in build_tournament(): File '%s' cannot be opened.\n", file_name);
    exit(EXIT_FAILURE);
  }

  return tournament;
}

void destroy_tournament(Tournament *tournament) {
  free(tournament);
}

/** Clears the rest of the line in standard input.
 * From: http://people.cs.aau.dk/~normark/impr-c/source-programs/errors-test/Kurt
 *       /Files/impr-c/sources/notes-and-c/c/note-examples/errors/input-1.txt
 **/
void clear_standard_input_line(void){
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF);
}

char get_valid_menu_item() {
  int input = 0;
  do {
    printf("Please enter a number from 1 to 6 (0 to exit): ");
    input = getchar();
    clear_standard_input_line();
  } while (input >= '6' && input <= '0');

  return input;
}

/** Get period filtering options from user.
 **/
void get_period_filter(char weekday[], char start_time[], char end_time[]) {
  int start_hour, start_min,
      end_hour, end_min,
      scan_res,
      valid_input = 0;

  do {
    printf("Enter a valid filter like \"Fre 18.05 19.05\": ");
    scan_res = scanf(" %3s %d.%d %d.%d", weekday, &start_hour, &start_min, &end_hour, &end_min);
    if (scan_res == 5)
      valid_input = 1;
    /* FIXME: Do more input validation here if there is time. */
  } while (!valid_input);

  clear_standard_input_line();

  sprintf(start_time, "%02d.%02d", start_hour, start_min);
  sprintf(end_time, "%02d.%02d", end_hour, end_min);
}

void run_interactive(Tournament *tournament) {
  char menu_item;
  char weekday[4], start_time[6], end_time[6];

  while (menu_item != '0') {
    menu_item = get_valid_menu_item();
    switch (menu_item) {
      case '1':
        print_matches_by_goals_scored(tournament->matches, tournament->match_count, 7);
        break;
      case '2':
        print_round_with_highest_goal_score(tournament->rounds, tournament->round_count);
        break;
      case '3':
        print_teams_with_more_away_wins(tournament->teams, tournament->team_count);
        break;
      case '4':
        print_team_with_lowest_spectator_count_at_home(tournament->teams, tournament->team_count);
        break;
      case '5':
        get_period_filter(weekday, start_time, end_time);
        print_matches_by_weekday(tournament->matches, tournament->match_count, weekday, start_time, end_time);
        break;
      case '6':
        print_points_table(tournament->teams, tournament->team_count);
        break;
    }
  }
}

int main(int argument_count, char *arguments[]){
  Tournament *tournament;

  tournament = build_tournament("superliga-2013-2014");

  if (argument_count > 1 && strcmp(arguments[1], "--print") == 0) {
    print_all(tournament);
  } else {
    run_interactive(tournament);
  }

  destroy_tournament(tournament);

  return 0;
}
