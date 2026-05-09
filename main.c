#include "banking.h"

int main(void)
{
    srand((unsigned)time(NULL));

    printf("============================================================\n");
    printf("   CONCURRENT BANKING SYSTEM    \n");
    printf("============================================================\n\n");

    init_accounts();
    print_accounts();

    printf("\n============================================================\n");
    printf("   MODULE 1 : THREADING & SCHEDULING                        \n");
    printf("============================================================\n");

    Customer base[] = {
        {1, REGULAR,   REGULAR,   "Ahmed",    0, 6, 0, 0, 0, 0, 0, {0}, 0},
        {2, PREMIUM,   PREMIUM,   "Sara",     2, 4, 0, 0, 0, 0, 0, {0}, 0},
        {3, LOAN,      LOAN,      "Hamza",    4, 7, 0, 0, 0, 0, 0, {0}, 0},
        {4, CORPORATE, CORPORATE, "Fatima",   1, 5, 0, 0, 0, 0, 0, {0}, 0},
        {5, VIP,       VIP,       "Ali",      3, 3, 0, 0, 0, 0, 0, {0}, 0},
    };
    int n = sizeof(base) / sizeof(base[0]);

    Customer c[10];
    memcpy(c, base, sizeof(base));
    run_fcfs(c, n);

    memcpy(c, base, sizeof(base));
    run_priority(c, n);

    memcpy(c, base, sizeof(base));
    run_round_robin(c, n);

    run_sync_demo();

    init_banker();
    run_banker_demo();

    run_ipc_demo();
    run_pipe_demo();

    run_memory_demo();

    run_vip_preemption_demo();

    printf("\n============================================================\n");
    printf("   FINAL ACCOUNT BALANCES                                   \n");
    printf("============================================================\n");
    print_accounts();

    sem_destroy(&deposit_sem);
    printf("\n[DONE] All modules completed successfully.\n");
    
    return 0;
}