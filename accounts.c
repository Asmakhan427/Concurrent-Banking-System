#include "banking.h"

Account         accounts[MAX_ACCOUNTS];
int             num_accounts = 0;
pthread_mutex_t account_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t           deposit_sem;

void init_accounts(void)
{
    sem_init(&deposit_sem, 0, 2);

    accounts[0] = (Account){101, 5000.0, "Omar"};
    accounts[1] = (Account){102, 3000.0, "Aisha"};
    accounts[2] = (Account){103, 8000.0, "Ibrahim"};
    accounts[3] = (Account){104, 1200.0, "Mariam"};
    accounts[4] = (Account){105, 9500.0, "Hassan"};
    num_accounts = 5;

    printf("[ACCOUNTS] Initialized %d accounts.\n", num_accounts);
}

static int find_account(int id)
{
    for (int i = 0; i < num_accounts; i++)
        if (accounts[i].id == id) return i;
    return -1;
}

int deposit(int account_id, double amount, const char *who)
{
    sem_wait(&deposit_sem);
    pthread_mutex_lock(&account_mutex);

    int idx = find_account(account_id);
    if (idx < 0) {
        pthread_mutex_unlock(&account_mutex);
        sem_post(&deposit_sem);
        printf("[DEPOSIT]  %-12s -> Account %d not found!\n", who, account_id);
        return -1;
    }
    accounts[idx].balance += amount;
    printf("[DEPOSIT]  %-12s deposited $%.2f  -> Account %d balance: $%.2f\n",
           who, amount, account_id, accounts[idx].balance);

    pthread_mutex_unlock(&account_mutex);
    sem_post(&deposit_sem);
    return 0;
}

int withdraw(int account_id, double amount, const char *who)
{
    pthread_mutex_lock(&account_mutex);

    int idx = find_account(account_id);
    if (idx < 0) {
        pthread_mutex_unlock(&account_mutex);
        printf("[WITHDRAW] %-12s -> Account %d not found!\n", who, account_id);
        return -1;
    }
    if (accounts[idx].balance < amount) {
        printf("[WITHDRAW] %-12s -> Insufficient funds (balance $%.2f, requested $%.2f)\n",
               who, accounts[idx].balance, amount);
        pthread_mutex_unlock(&account_mutex);
        return -1;
    }
    accounts[idx].balance -= amount;
    printf("[WITHDRAW] %-12s withdrew  $%.2f  -> Account %d balance: $%.2f\n",
           who, amount, account_id, accounts[idx].balance);

    pthread_mutex_unlock(&account_mutex);
    return 0;
}

void print_accounts(void)
{
    printf("\n+--------+--------------+-------------------+\n");
    printf("|  Acct  |   Owner      |   Balance ($)     |\n");
    printf("+--------+--------------+-------------------+\n");
    for (int i = 0; i < num_accounts; i++)
        printf("|  %4d  |  %-12s|  %15.2f  |\n",
               accounts[i].id, accounts[i].owner, accounts[i].balance);
    printf("+--------+--------------+-------------------+\n");
}