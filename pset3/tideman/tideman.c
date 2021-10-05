#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool is_cycle(int winner, int loser, int target);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
// rank = 0 (most preferred), rank = candidate_count - 1 (least preferred)
bool vote(int rank, string name, int ranks[])
{
    // iterate through each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // check if name matches candidate's name (not casesensitive)
        if (strcasecmp(candidates[i], name) == 0)
        {
            // insert candidate's id (i) into ranks and exit function
            ranks[rank] = i;
            return true;
        }
    }

    // if this line is reached, then the name was invalid
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // iterate through possible candidate comparisons
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i == j) // comparing a candidate to themselve = 0
            {
                preferences[i][j] = 0;
            }
            else
            {
                // get rank of candidate[i] and candidate[j]
                int i_rank, j_rank;
                for (int k = 0; k < candidate_count; k++)
                {
                    if (ranks[k] == i)
                    {
                        i_rank = k;
                    }
                    else if (ranks[k] == j)
                    {
                        j_rank = k;
                    }
                }

                // increment preferences if i is preferred over j
                if (i_rank < j_rank)
                {
                    preferences[i][j] += 1;
                }
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // only look at preferences of two unique candidates
            if (i != j)
            {
                // determine winner and loser, then add to pair
                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                    pair_count++;
                }
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
// using bubble sort since max pairs is 36
void sort_pairs(void)
{
    int swap_count;

    do
    {
        swap_count = 0;

        // iterate through pairs
        for (int i = 0; i <= pair_count - 2; i++)
        {
            int current_pair_strength, next_pair_strength;
            current_pair_strength = preferences[pairs[i].winner][pairs[i].loser];
            next_pair_strength = preferences[pairs[i + 1].winner][pairs[i + 1].loser];

            // if out of order, swap
            if (current_pair_strength < next_pair_strength)
            {
                pair temp = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = temp;
                swap_count++;
            }
        }
    }
    while (swap_count != 0);

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // lock first pair by default
    locked[pairs[0].winner][pairs[0].loser] = true;

    // iterate pairs
    for (int i = 0; i < pair_count; i++)
    {
        // skip if cycle, otherwise create edge
        if (!is_cycle(pairs[i].winner, pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    // iterate through each candidate
    // whoever has all their locked false is the source
    for (int i = 0; i < candidate_count; i++)
    {
        int locked_false_count = 0;

        // check if an arrow was pointed at candidate
        for (int j = 0; j < candidate_count; j++)
        {
            if (!locked[j][i]) // check for false
            {
                locked_false_count++;
            }
        }

        // if arrow never pointed at candidate, then they are the source
        if (locked_false_count == candidate_count)
        {
            // print winner and exit
            printf("%s\n", candidates[i]);
            return;
        }
    }

    return;
}

// Using recursion, check if the edge we're about to add will not cause a cycle
bool is_cycle(int winner, int loser, int target)
{
    // BASE CASE
    // if the loser and target are the same, then that means a cycle has been found
    if (loser == target)
    {
        return true;
    }

    // check if loser has any edges
    for (int i = 0; i < candidate_count; i++)
    {
        int new_winner = loser;
        int new_loser = i;

        // if edge is found, check that edge for more edges
        // otherwise continue checking for edges
        if (locked[new_winner][new_loser])
        {
            if (is_cycle(new_winner, new_loser, target))
            {
                return true;
            }
        }
    }

    // BASE CASE
    // every pair was checked and no cycle was found
    return false;
}