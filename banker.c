#include "banking.h"

static int available[NUM_RESOURCES];
static int maximum[MAX_CUSTOMERS][NUM_RESOURCES];
static int allocation[MAX_CUSTOMERS][NUM_RESOURCES];
static int need[MAX_CUSTOMERS][NUM_RESOURCES];

static const char *cnames[MAX_CUSTOMERS] = {
    "Zain", "Sara", "Hamza", "Fatima", "Ali"
};

void init_banker(void)
{
    int total[NUM_RESOURCES] = {10, 5, 7};

    int max_init[MAX_CUSTOMERS][NUM_RESOURCES] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };
    int alloc_init[MAX_CUSTOMERS][NUM_RESOURCES] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int used[NUM_RESOURCES] = {0};
    for (int i = 0; i < MAX_CUSTOMERS; i++)
        for (int j = 0; j < NUM_RESOURCES; j++) {
            maximum[i][j]    = max_init[i][j];
            allocation[i][j] = alloc_init[i][j];
            need[i][j]       = maximum[i][j] - allocation[i][j];
            used[j]         += allocation[i][j];
        }
    for (int j = 0; j < NUM_RESOURCES; j++)
        available[j] = total[j] - used[j];

    printf("[BANKER] Initialized. Available: [%d %d %d]\n",
           available[0], available[1], available[2]);
}

static int is_safe(int safe_seq[])
{
    int work[NUM_RESOURCES];
    int finish[MAX_CUSTOMERS] = {0};
    for (int j = 0; j < NUM_RESOURCES; j++) work[j] = available[j];

    int count = 0;
    while (count < MAX_CUSTOMERS) {
        int found = 0;
        for (int i = 0; i < MAX_CUSTOMERS; i++) {
            if (finish[i]) continue;
            int can = 1;
            for (int j = 0; j < NUM_RESOURCES; j++)
                if (need[i][j] > work[j]) { can = 0; break; }
            if (can) {
                for (int j = 0; j < NUM_RESOURCES; j++)
                    work[j] += allocation[i][j];
                finish[i]          = 1;
                safe_seq[count++]  = i;
                found = 1;
            }
        }
        if (!found) return 0;
    }
    return 1;
}

static void print_state(void)
{
    printf("\n%-12s  %-14s  %-14s  %-14s\n",
           "Customer", "Allocation", "Maximum", "Need");
    printf("%-12s  %-14s  %-14s  %-14s\n",
           "----------", "----------", "----------", "----------");
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        printf("%-12s  [%d %d %d]         [%d %d %d]         [%d %d %d]\n",
               cnames[i],
               allocation[i][0], allocation[i][1], allocation[i][2],
               maximum[i][0],    maximum[i][1],    maximum[i][2],
               need[i][0],       need[i][1],       need[i][2]);
    }
    printf("Available:    [%d %d %d]\n",
           available[0], available[1], available[2]);
}

int request_loan(int cid, int req[])
{
    printf("\n[BANKER] %s requests [%d %d %d]\n",
           cnames[cid], req[0], req[1], req[2]);

    for (int j = 0; j < NUM_RESOURCES; j++) {
        if (req[j] > need[cid][j]) {
            printf("[BANKER] DENIED: request exceeds declared maximum need.\n");
            return 0;
        }
    }
    for (int j = 0; j < NUM_RESOURCES; j++) {
        if (req[j] > available[j]) {
            printf("[BANKER] WAITING: not enough resources available.\n");
            return 0;
        }
    }

    for (int j = 0; j < NUM_RESOURCES; j++) {
        available[j]      -= req[j];
        allocation[cid][j] += req[j];
        need[cid][j]      -= req[j];
    }

    int safe_seq[MAX_CUSTOMERS];
    if (is_safe(safe_seq)) {
        printf("[BANKER] SAFE state. Loan GRANTED.\n");
        printf("[BANKER] Safe sequence: ");
        for (int k = 0; k < MAX_CUSTOMERS; k++)
            printf("%s%s", cnames[safe_seq[k]],
                   k < MAX_CUSTOMERS - 1 ? " -> " : "\n");
        return 1;
    }

    for (int j = 0; j < NUM_RESOURCES; j++) {
        available[j]      += req[j];
        allocation[cid][j] -= req[j];
        need[cid][j]      += req[j];
    }
    printf("[BANKER] UNSAFE state detected. Loan DENIED.\n");
    return 0;
}

static void release_resources(int cid, int rel[])
{
    printf("\n[BANKER] %s releases [%d %d %d]\n",
           cnames[cid], rel[0], rel[1], rel[2]);
    for (int j = 0; j < NUM_RESOURCES; j++) {
        allocation[cid][j] -= rel[j];
        available[j]       += rel[j];
        need[cid][j]       += rel[j];
    }
    printf("[BANKER] Updated available: [%d %d %d]\n",
           available[0], available[1], available[2]);
}

void run_banker_demo(void)
{
    printf("\n==========  MODULE 3 : BANKER'S ALGORITHM ==========\n");
    print_state();

    int r1[] = {1, 0, 0};
    request_loan(2, r1);
    print_state();

    int r2[] = {7, 4, 3};
    request_loan(0, r2);

    int r3[] = {0, 1, 0};
    request_loan(1, r3);

    int rel[] = {1, 0, 0};
    release_resources(2, rel);
    print_state();
}