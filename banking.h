#ifndef BANKING_H
#define BANKING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h>
#include <limits.h>
#include <signal.h>
#include <errno.h>

/* Customer Types / Priorities */
#define REGULAR      1
#define PREMIUM      3
#define LOAN         2
#define CORPORATE    4
#define VIP          5

/* Scheduling */
#define TIME_QUANTUM 2
#define MAX_CUSTOMERS 10

/* Banker's Algorithm */
#define NUM_RESOURCES 3

/* Memory Management */
#define NUM_FRAMES    4
#define MAX_PAGES_PER_CUSTOMER 5

/* IPC */
#define MSG_SIZE      256
#define PIPE_READ     0
#define PIPE_WRITE    1

/* Accounts */
#define MAX_ACCOUNTS  5
#define PAYROLL_THREADS 50

typedef struct {
    int    id;
    double balance;
    char   owner[32];
} Account;

typedef struct {
    int    id;
    int    type;
    int    priority;
    char   name[32];
    int    arrival_time;
    int    burst_time;
    int    start_time;
    int    finish_time;
    int    waiting_time;
    int    turnaround_time;
    int    response_time;
    int    memory_pages[MAX_PAGES_PER_CUSTOMER];
    int    num_memory_pages;
} Customer;

typedef struct {
    int    customer_id;
    char   customer_name[32];
    int    page_ids[MAX_PAGES_PER_CUSTOMER];
    int    num_pages;
} MemoryFootprint;

extern Account         accounts[MAX_ACCOUNTS];
extern int             num_accounts;
extern pthread_mutex_t account_mutex;
extern sem_t           deposit_sem;

void init_accounts(void);
int  deposit(int account_id, double amount, const char *who);
int  withdraw(int account_id, double amount, const char *who);
void print_accounts(void);

void run_fcfs(Customer *c, int n);
void run_priority(Customer *c, int n);
void run_round_robin(Customer *c, int n);
void print_metrics(Customer *c, int n);

void run_sync_demo(void);

void init_banker(void);
void run_banker_demo(void);
int  request_loan(int cid, int req[]);

void run_ipc_demo(void);
void run_pipe_demo(void);
void run_memory_demo(void);
void run_vip_preemption_demo(void);

#endif