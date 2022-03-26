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
void print_rank(int ranks[])
{
    for (int k = 0; k < candidate_count; k++)
        printf("%i\t", ranks[k]);

    printf("\n\n");
}
void print_references(void)
{
    for (int ii = 0; ii < candidate_count; ii++)
    {
        for (int j = 0; j < candidate_count; j++)
            printf("%i\t", preferences[ii][j]);

        printf("\n");
    }
    printf("\n");
}
void print_pairs(void)
{
    for (int ii = 0; ii < pair_count; ii++)
        printf("%i\t%i\n", pairs[ii].winner, pairs[ii].loser);

    printf("\n");
}

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

        print_rank(ranks);
        record_preferences(ranks);
        print_references();
        printf("\n");
    }

    add_pairs();
    print_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // bubble sort
    // TODO CONTINUE FROM HERE
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

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}