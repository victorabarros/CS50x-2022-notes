#include <cs50.h>
#include <stdio.h>
#include <string.h>

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
} pair;

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

// TODO REMOVE
// int votes[9][3] = {
//    {0, 1, 2},
//    {0, 1, 2},
//    {0, 1, 2},
//    {1, 2, 0},
//    {1, 2, 0},
//    {2, 0, 1},
//    {2, 0, 1},
//    {2, 0, 1},
//    {2, 0, 1}};
// void print_references(void)
//{
//    for (int ii = 0; ii < candidate_count; ii++)
//    {
//        for (int j = 0; j < candidate_count; j++)
//            printf("%i\t", preferences[ii][j]);
//
//        printf("\n");
//    }
//    printf("\n");
//}
// void print_pairs(void)
//{
//    for (int ii = 0; ii < pair_count; ii++)
//        printf("%i\t%i\n", pairs[ii].winner, pairs[ii].loser);
//
//    printf("\n");
//}
// void print_locked(void)
//{
//    // Clear graph of locked in pairs
//    for (int i = 0; i < candidate_count; i++)
//    {
//        for (int j = 0; j < candidate_count; j++)
//            printf(locked[i][j] ? "true\t" : "false\t");
//
//        printf("\n");
//    }
//
//    printf("\n");
//}

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
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         locked[i][j] = false;
    //     }
    // }

    // print_locked();

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");
    // int voter_count = 9;

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);
            // string name = candidates[votes[i][j]];
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

// Print the winner of the election
void print_winner(void)
{
    int winner;
    int winners = 0;

    while (winners != 1)
    {
        for (int i = 0; i < candidate_count; i++)
        {
            int falses = 0;

            for (int j = 0; j < candidate_count; j++)
            {
                if (locked[j][i])
                {
                    break;
                }

                falses++;
            }

            if (falses == candidate_count)
            {
                winners++;
                winner = i;
            }
        }

        if (winners == 0)
        {
            pair_count--;
            lock_pairs();
        }
    }
    printf("%s\n", candidates[winner]);
    return;
}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    for (int i = 0; i < pair_count; i++)
    {
        int winner_i = pairs[i].winner;
        int loser_i = pairs[i].loser;

        locked[winner_i][loser_i] = true;
    }
    // print_locked();
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int swaps = 1;
    while (swaps != 0)
    {
        swaps = 0;
        for (int i = 1; i < pair_count; i++)
        {
            int winner_i_minus = pairs[i - 1].winner;
            int loser_i_minus = pairs[i - 1].loser;

            int height_i_minus = preferences[winner_i_minus][loser_i_minus] - preferences[loser_i_minus][winner_i_minus];

            int winner_i = pairs[i].winner;
            int loser_i = pairs[i].loser;

            int height_i = preferences[winner_i][loser_i] - preferences[loser_i][winner_i];

            if (height_i_minus < height_i)
            {
                pairs[i].winner = winner_i_minus;
                pairs[i].loser = loser_i_minus;

                pairs[i - 1].winner = winner_i;
                pairs[i - 1].loser = loser_i;

                swaps++;
            }
        }
    }

    // print_pairs();
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int index = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int i_against_j = preferences[i][j];
            int j_against_i = preferences[j][i];

            if (i_against_j == j_against_i)
                continue;

            int winner = i;
            int loser = i;

            if (i_against_j > j_against_i)
                loser = j;
            else
                winner = j;

            pairs[index].winner = winner;
            pairs[index].loser = loser;

            index++;
        }
    }

    pair_count = index;
    // print_pairs();
    return;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int winner = ranks[i];
            int loser = ranks[j];
            preferences[winner][loser]++;
        }
    }
    // print_references();
    return;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}
