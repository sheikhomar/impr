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

/* Define limits. These values can be increased if needed */
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

  /* The sole function of str_spectator_count is to facilitate
   * number conversion. */
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
  /* Total amount of spectators in home games */
  int home_spectator_sum;
  int won_matches;
  int tied_matches;
  int lost_matches;
  /* Total amount of goals scored throughout the tournament */
  int goals_scored;
  /* Total amount of goals opponent teams have scored in the tournament */
  int goals_against;
  int goals_diff;
  int points;
};
typedef struct team Team;

/** Represents a match in a tournament.
 *
 * This structure looks very similar to the LineInfo structure.
 * The big difference is that a Match structure contains pointers
 * Team structures.
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
 **/
struct tournament {
  Match *matches[MAX_MATCHES_PER_ROUND * MAX_ROUNDS];
  int match_count;
  Team *teams[MAX_TEAMS];
  int team_count;
  Round *rounds[MAX_ROUNDS];
  int round_count;
};
typedef struct tournament Tournament;

struct match_collection {
  int count;
  Match *matches[MAX_MATCHES_PER_ROUND * MAX_ROUNDS];
};
typedef struct match_collection MatchCollection;

struct team_collection {
  int count;
  Team *teams[MAX_TEAMS];
};
typedef struct team_collection TeamCollection;

/**
 * The following functions are categorized into two major groups:
 *  1) functions that deal with parsing the input file and building
 *     a complete C-structure containing all the information in
 *     the input file. Input processing starts in build_tournament()
 *
 *  2) functions that filters or sorts data and presents the
 *     processed data to the user via the standard output.
 **/

/**  1) Input processing functions */
/* 1.1) Build tournament */
Tournament *build_tournament(const char file_name[]);
Tournament *create_tournament(void);
void initialize_tournament(Tournament *tournament);

/* 1.2) Build rounds */
void build_round_matches(Tournament *tournament, char match_lines[][MAX_CHARS_PER_LINE], int line_count);
Round *create_round(Tournament *tournament);
void initialize_round(Round *round, int round_number);

/* 1.3) Parse */
int is_end_marker_for_round(char *line);
LineInfo *parse_line(const char line[]);
int convert_to_int(const char input[]);

/* 1.4) Build matches */
Match *create_match(Tournament *tournament, int round, LineInfo *line_info);
void initialize_match(Match *match, LineInfo *line_info);

/* 1.5) Build teams */
Team *find_or_create_team(Tournament *tournament, const char team_name[]);
Team *find_team_by_name(Team *teams[], int team_count, const char team_name[]);
Team *create_team(Team *teams[], int *team_count, const char team_name[]);
void initialize_team(Team *team, const char team_name[]);

/* 1.6) Post-process */
void update_tournament_stats(Tournament *tournament);
void update_match_stats_for_teams(Match *matches[], int match_count);
void compute_team_stats(Team *teams[], int team_count);

/* 1.7) Release allocated memory */
void free_memory(Tournament *tournament);

/**  2) Output related functions */
/* 2.1) Print functions */
void print_all(Tournament *tournament);
void print_interactive(Tournament *tournament);
void print_matches_by_goals_scored(Tournament *tournament, int goals);
void print_round_with_highest_goal_score(Round *rounds[], int round_count);
void print_teams_with_more_away_wins(Tournament *tournament);
void print_team_with_lowest_spectator_count_at_home(Tournament *tournament);
void print_matches_by_weekday(Tournament *tournament,
    const char *weekday, const char start_time[], const char end_time[]);
void print_points_table(Team *teams[], int team_count);
void print_match(const Match *match);

/* 2.2) Filter and sort functions */
Round *find_round_with_highest_goal_count(Round *rounds[], int round_count);
void filter_matches_by_goals(Tournament *tournament, int goals, MatchCollection *filtered_matches);
void find_teams_with_more_away_than_home_wins(Tournament *tournament, TeamCollection *output);
Team *find_team_with_lowest_spectator_count_in_home_games(Team *teams[], int team_count);
void filter_and_sort_matches_by_weekday_and_time(Tournament *tournament, MatchCollection *output,
    const char *weekday, const char start_time[], const char end_time[]);

/* 2.3) Helper functions used in print_interactive() */
void print_menu(void);
char get_valid_menu_selection(void);
void get_period_filter(char weekday[], char start_time[], char end_time[]);
void clear_standard_input_line(void);
void parse_time(const char formatted_time[], int *hours, int *minutes);
int get_minutes_since_midnight(int hours, int minutes);
int parse_time_as_minutes_since_midnight(const char formatted_time[]);

/* 2.4) Compare functions used to sort data before writing to stdout */
int compare_rounds_by_total_goals(const void *a, const void *b);
int compare_teams_by_spectator_sum(const void *a, const void *b);
int compare_matches_by_total_goals(const void *a, const void *b);
int compare_teams_by_points(const void *a, const void *b);


int main(int argument_count, char *arguments[]){
  Tournament *tournament;
  int auto_run;

  /* Parse input */
  tournament = build_tournament("superliga-2013-2014");
  auto_run = argument_count > 1 && strcmp(arguments[1], "--print") == 0;

  /* Output processed data */
  if (auto_run) {
    print_all(tournament);
  } else {
    print_interactive(tournament);
  }

  free_memory(tournament);

  return EXIT_SUCCESS;
}


void read_file(const char file_name[]) {
  char line[MAX_CHARS_PER_LINE];
  int line_count = 0;
  FILE *handle = fopen(file_name, "r");

  if (handle != NULL) {
    while (fgets(line, MAX_CHARS_PER_LINE, handle) != NULL) {
      line_count++;
    }
    fclose(handle);
  } else {
    printf("Error in read_file(): File '%s' cannot be opened.\n", file_name);
    exit(EXIT_FAILURE);
  }

}

/** Build a new tournament using data from file.
 **/
Tournament *build_tournament(const char file_name[]) {
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

/** Creates a new tournament.
 */
Tournament *create_tournament(void) {
  Tournament *tournament = NULL;

  tournament = (Tournament *)malloc(sizeof(Tournament));

  if (tournament == NULL) {
    printf("Error in create_tournament(): Could not allocate memory!");
    exit(EXIT_FAILURE);
  }

  initialize_tournament(tournament);

  return tournament;
}

/** Initializes a Tournament structure with default values.
 **/
void initialize_tournament(Tournament *tournament) {
  tournament->match_count = 0;
  tournament->team_count = 0;
  tournament->round_count = 0;
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

/** Creates a new round in the tournament.
 **/
Round *create_round(Tournament *tournament) {
  Round *round;

  /* Make sure a new round can be created. */
  assert(tournament->round_count < MAX_ROUNDS);

  round = (Round *)malloc(sizeof(Round));

  if (round == NULL) {
    printf("Error in create_round(): Could not allocate memory!");
    exit(EXIT_FAILURE);
  }

  tournament->rounds[tournament->round_count] = round;
  tournament->round_count++;

  initialize_round(round, tournament->round_count);

  return round;
}

/** Initializes a Round structure with default values.
 **/
void initialize_round(Round *round, int round_number) {
  round->number = round_number;
  round->total_goals = 0;
}

/** Determines whether the given line marks the end of a round in the input file.
 **/
int is_end_marker_for_round(char *line) {
  return strcmp(line, "\r\n") == 0;
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

  /* Make sure line is formatted correctly! */
  assert(scan_res == 10);

  line_info.spectator_count = convert_to_int(line_info.str_spectator_count);

  return &line_info;
}

/** Converts a formatted string of an integer to integer.
 **/
int convert_to_int(const char input[]) {
  double return_value = 0.0;

  sscanf(input, "%lf", &return_value);

  return return_value * 1000;
}

/** Creates a match in the tournament.
 **/
Match *create_match(Tournament *tournament, int round, LineInfo *line_info) {
  Match *match;

  assert(tournament->match_count < MAX_MATCHES_PER_ROUND * MAX_ROUNDS);

  match = (Match *)malloc(sizeof(Match));

  if (match == NULL) {
    printf("Error in create_match(): Could not allocate memory!");
    exit(EXIT_FAILURE);
  }

  tournament->matches[tournament->match_count] = match;
  tournament->match_count++;

  initialize_match(match, line_info);

  match->home_team = find_or_create_team(tournament, line_info->home_team);
  match->away_team = find_or_create_team(tournament, line_info->away_team);
  match->round = round;

  return match;
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

/** Finds a team in the tournament by name. If the team is
 *  not found, a new team is created in the tournament.
 **/
Team *find_or_create_team(Tournament *tournament, const char team_name[]) {
  Team *team = NULL;

  team = find_team_by_name(tournament->teams, tournament->team_count, team_name);

  if (!team) {
    team = create_team(tournament->teams, &tournament->team_count, team_name);
  }

  return team;
}

/** Finds a team in the tournament. If the team doesn't exist
 * NULL is returned.
 **/
Team *find_team_by_name(Team *teams[], int team_count, const char team_name[]) {
  int i;

  for (i = 0; i < team_count; i++)
    if (strcmp(teams[i]->name, team_name) == 0)
      return teams[i];

  return NULL;
}

/** Creates a new team in the tournament.
 **/
Team *create_team(Team *teams[], int *team_count, const char team_name[]) {
  Team *team = NULL;

  assert(*team_count < MAX_TEAMS);

  team = (Team *)malloc(sizeof(Team));

  if (team == NULL) {
    printf("Error in create_team(): Could not allocate memory!");
    exit(EXIT_FAILURE);
  }

  teams[*team_count] = team;
  (*team_count)++;

  initialize_team(team, team_name);

  return team;
}

/** Initializes a Team structure with the given name.
 **/
void initialize_team(Team *team, const char team_name[]) {
  strcpy(team->name, team_name);
  team->home_wins = 0;
  team->away_wins = 0;
  team->home_spectator_sum = 0;
  team->won_matches = 0;
  team->tied_matches = 0;
  team->lost_matches = 0;
  team->goals_scored = 0;
  team->goals_against = 0;
  team->goals_diff = 0;
  team->points = 0;
}

/** Updates tournament statistics.
 **/
void update_tournament_stats(Tournament *tournament) {
  update_match_stats_for_teams(tournament->matches, tournament->match_count);
  compute_team_stats(tournament->teams, tournament->team_count);
}

/** Updates match statistics.
**/
void update_match_stats_for_teams(Match *matches[], int match_count) {
  int i;
  Match *match;
  Team *home_team, *away_team;

  for (i = 0; i < match_count; i++) {
    /* Set convenience variables */
    match = matches[i];
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
    home_team->home_spectator_sum += match->spectator_count;
  }
}

/** Computes goals and points for each team.
 **/
void compute_team_stats(Team *teams[], int team_count) {
  int i;

  for (i = 0; i < team_count; i++) {
    teams[i]->goals_diff = teams[i]->goals_scored - teams[i]->goals_against;
    teams[i]->points = teams[i]->won_matches * 3 + teams[i]->tied_matches;
  }
}

void free_memory(Tournament *tournament) {
  int i;

  for (i = 0; i < tournament->round_count; i++)
    free(tournament->rounds[i]);

  for (i = 0; i < tournament->team_count; i++)
    free(tournament->teams[i]);

  for (i = 0; i < tournament->match_count; i++)
    free(tournament->matches[i]);

  free(tournament);
}

/** Prints out all solutions with default input.
 **/
void print_all(Tournament *tournament) {
  printf("Running program with --print argument.");
  print_matches_by_goals_scored(tournament, 7);
  print_round_with_highest_goal_score(tournament->rounds, tournament->round_count);
  print_teams_with_more_away_wins(tournament);
  print_team_with_lowest_spectator_count_at_home(tournament);
  print_matches_by_weekday(tournament, "Fre", "18.05", "19.05");
  print_points_table(tournament->teams, tournament->team_count);
}

void print_interactive(Tournament *tournament) {
  char menu_selection = 0;
  char weekday[4], start_time[6], end_time[6];

  while (menu_selection != '0') {
    print_menu();
    menu_selection = get_valid_menu_selection();
    switch (menu_selection) {
      case '1':
        print_matches_by_goals_scored(tournament, 7);
        break;
      case '2':
        print_round_with_highest_goal_score(tournament->rounds, tournament->round_count);
        break;
      case '3':
        print_teams_with_more_away_wins(tournament);
        break;
      case '4':
        print_team_with_lowest_spectator_count_at_home(tournament);
        break;
      case '5':
        get_period_filter(weekday, start_time, end_time);
        print_matches_by_weekday(tournament, weekday, start_time, end_time);
        break;
      case '6':
        print_points_table(tournament->teams, tournament->team_count);
        break;
    }
  }
}


/** Prints a list of matches filtered by goals.
 **/
void print_matches_by_goals_scored(Tournament *tournament, int goals) {
  int i;
  MatchCollection filtered_matches;

  filter_matches_by_goals(tournament, goals, &filtered_matches);

  printf("\n\n1) Matches with %d or more goals:\n\n", goals);
  for (i = 0; i < filtered_matches.count; i++)
    print_match(filtered_matches.matches[i]);
}

/** Prints the round with the highest goal score.
 **/
void print_round_with_highest_goal_score(Round *rounds[], int round_count) {
  Round *round = find_round_with_highest_goal_count(rounds, round_count);

  printf("\n\n2) Round %d has the highest goal score with %d goals\n",
      round->number, round->total_goals);
}

/** Prints a list of teams that have more away wins than home wins.
 * */
void print_teams_with_more_away_wins(Tournament *tournament) {
  int i;
  TeamCollection filtered;

  find_teams_with_more_away_than_home_wins(tournament, &filtered);

  printf("\n\n3) Teams with more aways wins than home wins:\n");
  for (i = 0; i < filtered.count; i++) {
    printf(" - %s has %d away wins and %d home wins \n",
      filtered.teams[i]->name,
      filtered.teams[i]->away_wins,
      filtered.teams[i]->home_wins);
  }
}

void print_team_with_lowest_spectator_count_at_home(Tournament *tournament) {
  Team *team = find_team_with_lowest_spectator_count_in_home_games(
      tournament->teams, tournament->team_count);

  printf("\n\n4) The team with lowest spectator count in home games is %s with only %d spectators!\n",
      team->name, team->home_spectator_sum);
}

/** Prints a sorted list of matches that are played on a certain weekday
 *  and within a certain time period.
 */
void print_matches_by_weekday(Tournament *tournament, const char *weekday,
    const char start_time[], const char end_time[]) {

  int i;
  MatchCollection list;
  filter_and_sort_matches_by_weekday_and_time(tournament, &list, weekday, start_time, end_time);

  printf("\n\n5) Matches played on %s from %s to %s:\n\n", weekday, start_time, end_time);

  for (i = 0; i < list.count; i++)
    print_match(list.matches[i]);
}

/** Prints points table for alle teams.
 */
void print_points_table(Team *teams[], int team_count) {
  int i;

  /* Sort teams by points in descending order
   * FIXME: Move sorting into another function */
  qsort(teams, team_count, sizeof(Team *), compare_teams_by_points);

  /* Print out the score */
  printf("\n\n6) Team points table:\n\n");
  printf(" No  Team   W    T    L   G+    G-    +/-    P\n");
  printf("-----------------------------------------------\n");
  for (i = 0; i < team_count; i++) {
    printf("%3d  %-3s   %2d   %2d   %2d   %2d    %2d    %+3d   %2d\n",
        i+1,
        teams[i]->name,
        teams[i]->won_matches,
        teams[i]->tied_matches,
        teams[i]->lost_matches,
        teams[i]->goals_scored,
        teams[i]->goals_against,
        teams[i]->goals_diff,
        teams[i]->points
        );
  }
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

void print_menu(void) {
  /* Multiple line string idea from:
   * http://stackoverflow.com/questions/797318/how-to-split-a-string-literal-
   * across-multiple-lines-in-c-objective-c: */
  printf("\n\
==================================================================\n\
 Program options: \n\
==================================================================\n\
 1) Print matches where number of goals scored is 7 or more. \n\
 2) Print the round with the highest goal score. \n\
 3) Print teams with more away wins than home wins. \n\
 4) Print the team with the lowest spectator count in home games. \n\
 5) Find matches played on certain weekdays and time. \n\
 6) Print points table. \n\n\
 0) Exit program. \n\
");
}

/** Finds the round with the highest goal score.
 **/
Round *find_round_with_highest_goal_count(Round *rounds[], int round_count) {
  int i;
  Round *round = NULL;

  for (i = 0; i < round_count; i++)
    if (round == NULL || rounds[i]->total_goals > round->total_goals)
      round = rounds[i];

  return round;
}

/** Filter matches by goals.
 */
void filter_matches_by_goals(Tournament *tournament, int goals, MatchCollection *filtered_matches) {
  int i;
  filtered_matches->count = 0;
  for (i = 0; i < tournament->match_count; i++) {
    if (tournament->matches[i]->total_goals >= goals) {
      filtered_matches->matches[filtered_matches->count] = tournament->matches[i];
      filtered_matches->count++;
    }
  }
}

void find_teams_with_more_away_than_home_wins(Tournament *tournament, TeamCollection *output) {
  int i;

  output->count = 0;
  for (i = 0; i < tournament->team_count; i++) {
    if (tournament->teams[i]->away_wins > tournament->teams[i]->home_wins) {
      output->teams[output->count] = tournament->teams[i];
      output->count++;
    }
  }
}

Team *find_team_with_lowest_spectator_count_in_home_games(Team *teams[], int team_count) {
  int i, lowest_spectator_count = INT_MAX;
  Team *team = NULL;

  for (i = 0; i < team_count; i++) {
    if (teams[i]->home_spectator_sum < lowest_spectator_count) {
      lowest_spectator_count = teams[i]->home_spectator_sum;
      team = teams[i];
    }
  }

  return team;
}

void filter_and_sort_matches_by_weekday_and_time(Tournament *tournament, MatchCollection *output,
    const char *weekday, const char start_time[], const char end_time[]) {
  int i, filter_start_time, filter_end_time, match_time;
  Match *match;

  /* Setup time filters */
  filter_start_time = parse_time_as_minutes_since_midnight(start_time);
  filter_end_time = parse_time_as_minutes_since_midnight(end_time);

  output->count = 0;
  for (i = 0; i < tournament->match_count; i++) {
    match = tournament->matches[i];
    if (strcmp(match->weekday, weekday) == 0) {
      match_time = get_minutes_since_midnight(match->hour, match->minute);
      if (match_time >= filter_start_time && match_time <= filter_end_time) {
        output->matches[output->count] = match;
        output->count++;
      }
    }
  }

  /* Sort the list of matches by total goals in descending order */
  qsort(output->matches, output->count, sizeof(Match *), compare_matches_by_total_goals);
}

/** Get a valid menu selection from standard input
 **/
char get_valid_menu_selection(void) {
  int input = 0;
  do {
    printf("Please enter a number from 1 to 6 (0 to exit): ");

    /* Get only the first char and ignore the rest */
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

    clear_standard_input_line();
  } while (!valid_input);

  sprintf(start_time, "%02d.%02d", start_hour, start_min);
  sprintf(end_time, "%02d.%02d", end_hour, end_min);
}

/** Clears the rest of the line in standard input.
 * From: http://people.cs.aau.dk/~normark/impr-c/source-programs/errors-test/Kurt
 *       /Files/impr-c/sources/notes-and-c/c/note-examples/errors/input-1.txt
 **/
void clear_standard_input_line(void){
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF);
}

/** Parses a string that contains time in the format 'HH.MM'.
 **/
void parse_time(const char formatted_time[], int *hours, int *minutes) {
  int scan_res;

  scan_res = sscanf(formatted_time, " %d.%d", hours, minutes);

  /* Make sure the given formatted_time has the correct format. */
  assert(scan_res == 2);
}

/** Calculates amount of minutes since midnight.
 **/
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

/**
 * Compare two teams by spectators in home matches.
 **/
int compare_teams_by_spectator_sum(const void *a, const void *b) {
  Team *team1 = *((Team **)a);
  Team *team2 = *((Team **)b);

  return team1->home_spectator_sum - team2->home_spectator_sum;
}

/** Compares two matches by goals scored. Highest goal count is first.
 **/
int compare_matches_by_total_goals(const void *a, const void *b) {
  Match *match1 = *((Match **)a);
  Match *match2 = *((Match **)b);

  return match2->total_goals - match1->total_goals;
}

/** Compares two teams according to the specs.
 **/
int compare_teams_by_points(const void *a, const void *b) {
  Team *team1 = *((Team **)a);
  Team *team2 = *((Team **)b);

  /* Priority 1: Compare by points */
  int points_diff = team2->points - team1->points;
  /* Priority 2: Compare by goal difference when points are equal */
  int goals_diff = team2->goals_diff - team1->goals_diff;
  /* Priority 3: Compare by total goals scored when goal differences are equal */
  int goals_scored_diff = team2->goals_scored - team1->goals_scored;
  /* Priority 4: Compare by name when amount of goals scored are equal */
  int name_diff = strcmp(team2->name, team1->name);

  if (points_diff == 0) {
    if (goals_diff == 0) {
      if (goals_scored_diff == 0)
        return name_diff;
      else
        return goals_scored_diff;
    } else {
      return goals_diff;
    }
  }

  return points_diff;
}
