#include "banking.h"

typedef struct {
    int    account_id;
    double amount;
    char   name[32];
    char   action[16];
} TxArg;

static void *thread_worker(void *arg)
{
    TxArg *tx = (TxArg *)arg;
    usleep((rand() % 200 + 50) * 1000);

    if (strcmp(tx->action, "deposit") == 0)
        deposit(tx->account_id, tx->amount, tx->name);
    else
        withdraw(tx->account_id, tx->amount, tx->name);

    free(tx);
    return NULL;
}

static void *payroll_thread(void *arg)
{
    TxArg *tx = (TxArg *)arg;
    printf("[PAYROLL] Employee %s depositing $%.2f to Account %d\n",
           tx->name, tx->amount, tx->account_id);
    deposit(tx->account_id, tx->amount, tx->name);
    free(tx);
    return NULL;
}

void run_sync_demo(void)
{
    printf("\n==========  MODULE 2 : SYNCHRONISATION  ==========\n");
    printf("[INFO] Semaphore value=2: at most 2 deposits run concurrently.\n");
    printf("[INFO] Mutex: prevents race conditions on shared account balances.\n\n");

    pthread_t threads[100];
    int count = 0;

    struct { const char *name; int acct; double amt; } deps[] = {
        {"Bilal",   101, 500.0},
        {"Zainab",  102, 300.0},
        {"Usman",   104, 200.0},
    };
    for (int i = 0; i < 3; i++) {
        TxArg *tx = malloc(sizeof(TxArg));
        tx->account_id = deps[i].acct;
        tx->amount     = deps[i].amt;
        strncpy(tx->name,   deps[i].name, 31);
        strncpy(tx->action, "deposit",    15);
        pthread_create(&threads[count++], NULL, thread_worker, tx);
    }

    struct { const char *name; int acct; double amt; } wds[] = {
        {"Bilal-W1", 101, 100.0},
        {"Bilal-W2", 101, 200.0},
    };
    for (int i = 0; i < 2; i++) {
        TxArg *tx = malloc(sizeof(TxArg));
        tx->account_id = wds[i].acct;
        tx->amount     = wds[i].amt;
        strncpy(tx->name,   wds[i].name, 31);
        strncpy(tx->action, "withdraw",  15);
        pthread_create(&threads[count++], NULL, thread_worker, tx);
    }

    printf("\n[CORPORATE] Starting %d payroll threads for employees...\n", PAYROLL_THREADS);
    for (int i = 0; i < PAYROLL_THREADS; i++) {
        TxArg *tx = malloc(sizeof(TxArg));
        tx->account_id = 103;
        tx->amount     = 750.0;
        snprintf(tx->name, 31, "Emp-%d", i + 1);
        strncpy(tx->action, "deposit", 15);
        pthread_create(&threads[count++], NULL, payroll_thread, tx);
    }
    printf("[CORPORATE] All %d payroll threads created!\n", PAYROLL_THREADS);

    for (int i = 0; i < count; i++)
        pthread_join(threads[i], NULL);

    printf("\n[SYNC] All %d threads completed (including %d payroll threads).\n", 
           count, PAYROLL_THREADS);
    print_accounts();
}