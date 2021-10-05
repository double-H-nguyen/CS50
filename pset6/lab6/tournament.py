# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():
    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = read_csv_file(sys.argv[1])  # read in file and return a list of team dictionaries

    counts = initialize_dict(teams)  # returns dictionary where key=team name, value=number of times won

    # Simulate N tournaments and keep track of win counts
    for i in range(N):
        winner = simulate_tournament(teams)  # return team name that won the tournament
        if winner in counts:
            counts[winner] += 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    teams_ct = len(teams)  # get initial teams count

    # simulate round until 1 team is left
    while teams_ct != 1:
        teams = simulate_round(teams)  # return teams left
        teams_ct = len(teams)

    return teams[0]['team']  # return team name that won the  tournament


def read_csv_file(filename):
    teams_lst = []

    with open(filename, "r") as f:  # open tournament file
        reader = csv.DictReader(f)  # read and map each row as a dictionary

        # extract dictionary from each row and add to teams list
        for row in reader:
            row['rating'] = int(row['rating'])  # make sure rating is int
            teams_lst.append(row)

    return teams_lst


# take in list of team dictionaries and create a new dictionary
# key=team name, value=number of times won
def initialize_dict(teams_lst):
    teams_dict = {}

    # initialize value to 0
    for d in teams_lst:
        teams_dict[d['team']] = 0

    return teams_dict


if __name__ == "__main__":
    main()
