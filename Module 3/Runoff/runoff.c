#include <cs50.h>
#include <stdio.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name) //voter = i, rank = j
{
    // TODO
    for (int i = 0, n = candidate_count; i < n; i++)
    {
        if(name == candidates[i].name)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int i = 0, n = candidate_count; i < n; i++)
    {
        candidates[i].votes = 0;
    }
    int w = 0;
    for (int e = 0, m = voter_count; e < m; e++)
    {
        int i = preferences[e][0];
        if(!candidates[i].eliminated)
        {
            candidates[i].votes = candidates[i].votes + 1;
        }
        else
        {
            for (int j = 1, pret = candidate_count; j < pret; j++)
            {
                if(!candidates[j].eliminated)
                    candidates[j].votes = candidates[j].votes + 1;
            }
        }

    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO
    for(int i = 0, n = candidate_count ; i < n; i++)
    {
        if(candidates[i].votes > voter_count / 2)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    candidate subscand[MAX_CANDIDATES];
    for (int i = 0, n = candidate_count; i <= n; i++)
    {
        subscand[i] = candidates[i];
    }

    for (int i = 0, n = candidate_count; i <= n; i++)
    {
        for (int w = (i + 1), e = candidate_count; w <= e; w++)
        {
            if (candidates[i].votes < candidates[w].votes && candidates[w].name)
            {
                int a = candidates[w].votes;
                string v = candidates[w].name;
                candidates[w].votes = candidates[i].votes;
                candidates[w].name = candidates[i].name;
                candidates[i].votes = a;
                candidates[i].name = v;
            }
        }
    }

    for (int i = 0, n = candidate_count - 1; i < n; i++)
    {
        if (candidates[i].votes > candidates[i + 1].votes && candidates[i + 1].eliminated == true)
        {
            int themin = candidates[i].votes;
            for (int xy = 0, xa = candidate_count; xy <= xa; xy++)
                {
                    candidates[xy] = subscand[xy];
                }
            return themin;
        }
        else if (i == candidate_count - 2 && candidates[i].votes > candidates[i + 1].votes)
        {
            int themin = candidates[i + 1].votes;
            for (int xy = 0, xa = candidate_count; xy <= xa; xy++)
            {
                candidates[xy] = subscand[xy];
            }
            return themin;
        }
        else
        {
            return candidates[i].votes;
        }
    }
    return 0;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    int remaining = 0;
    for (int i = 0, n = candidate_count; i < n; i++)
    {
        if (!candidates[i].eliminated)
            remaining = remaining + 1;
    }

    for(int i = 0, n = remaining - 1; i < n; i++)
    {
        if (candidates[i].votes == candidates[i + 1].votes && candidates[i + 1].eliminated == false)
        {
            if(candidates[i + 1].votes == candidates[i + 2].votes && candidates[i + 2].eliminated == false)
                return true;
            else if(candidates[i + 2].eliminated != false)
                return true;
        }
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    for (int i = 0, n = candidate_count; i < n; i++)
    {
        if (candidates[i].votes == min)
            candidates[i].eliminated = true;
    }
    return;
}