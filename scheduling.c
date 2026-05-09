#include "banking.h"

static int cmp_arrival(const void *a, const void *b)
{
    return ((Customer *)a)->arrival_time - ((Customer *)b)->arrival_time;
}

static int cmp_priority_desc(const void *a, const void *b)
{
    int d = ((Customer *)b)->priority - ((Customer *)a)->priority;
    if (d != 0) return d;
    return ((Customer *)a)->arrival_time - ((Customer *)b)->arrival_time;
}

void print_metrics(Customer *c, int n)
{
    printf("\n%-14s %-10s %-10s %-12s %-14s %-16s %-14s\n",
           "Customer", "Priority", "Arrival", "Burst(ms)",
           "Waiting(ms)", "Turnaround(ms)", "Response(ms)");
    printf("%-14s %-10s %-10s %-12s %-14s %-16s %-14s\n",
           "-----------", "--------", "-------", "---------",
           "-----------", "--------------", "----------");

    double avg_wt = 0, avg_tat = 0, avg_rt = 0;
    for (int i = 0; i < n; i++) {
        printf("%-14s %-10d %-10d %-12d %-14d %-16d %-14d\n",
               c[i].name, c[i].priority, c[i].arrival_time,
               c[i].burst_time, c[i].waiting_time, c[i].turnaround_time,
               c[i].response_time);
        avg_wt  += c[i].waiting_time;
        avg_tat += c[i].turnaround_time;
        avg_rt  += c[i].response_time;
    }
    printf("\nAverage Waiting Time    : %.2f ms\n", avg_wt  / n);
    printf("Average Turnaround Time : %.2f ms\n",  avg_tat / n);
    printf("Average Response Time   : %.2f ms\n",  avg_rt  / n);
}

static void print_gantt(Customer *c, int n)
{
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < n; i++)
        printf(" %-9s|", c[i].name);
    printf("\n%d", c[0].start_time);
    for (int i = 0; i < n; i++)
        printf("%-11d", c[i].finish_time);
    printf("\n");
}

void run_fcfs(Customer *c, int n)
{
    printf("\n==========  FCFS SCHEDULING  ==========\n");
    qsort(c, n, sizeof(Customer), cmp_arrival);

    int t = 0;
    for (int i = 0; i < n; i++) {
        if (t < c[i].arrival_time) t = c[i].arrival_time;
        c[i].start_time      = t;
        c[i].finish_time     = t + c[i].burst_time;
        c[i].waiting_time    = c[i].start_time - c[i].arrival_time;
        c[i].turnaround_time = c[i].finish_time - c[i].arrival_time;
        c[i].response_time   = c[i].start_time - c[i].arrival_time;
        t = c[i].finish_time;
    }
    print_gantt(c, n);
    print_metrics(c, n);
}

void run_priority(Customer *c, int n)
{
    printf("\n==========  PRIORITY SCHEDULING  ==========\n");
    printf("Priority levels: VIP=5, Corporate=4, Premium=3, Loan=2, Regular=1\n");
    qsort(c, n, sizeof(Customer), cmp_priority_desc);

    int t = 0;
    for (int i = 0; i < n; i++) {
        if (t < c[i].arrival_time) t = c[i].arrival_time;
        c[i].start_time      = t;
        c[i].finish_time     = t + c[i].burst_time;
        c[i].waiting_time    = c[i].start_time - c[i].arrival_time;
        c[i].turnaround_time = c[i].finish_time - c[i].arrival_time;
        c[i].response_time   = c[i].start_time - c[i].arrival_time;
        t = c[i].finish_time;
    }
    print_gantt(c, n);
    print_metrics(c, n);
}

void run_round_robin(Customer *c, int n)
{
    printf("\n==========  ROUND-ROBIN SCHEDULING (quantum=%d ms)  ==========\n",
           TIME_QUANTUM);

    qsort(c, n, sizeof(Customer), cmp_arrival);

    int remaining[MAX_CUSTOMERS];
    for (int i = 0; i < n; i++) {
        remaining[i]     = c[i].burst_time;
        c[i].start_time  = -1;
        c[i].finish_time =  0;
    }

    char gantt_name[256][32];
    int  gantt_end[256];
    int  slots = 0;

    int t = 0, done = 0;
    while (done < n) {
        int progressed = 0;
        for (int i = 0; i < n; i++) {
            if (remaining[i] <= 0 || c[i].arrival_time > t) continue;
            progressed = 1;
            if (c[i].start_time == -1) {
                c[i].start_time = t;
                c[i].response_time = t - c[i].arrival_time;
            }

            int slice = (remaining[i] < TIME_QUANTUM) ? remaining[i] : TIME_QUANTUM;
            remaining[i] -= slice;
            t            += slice;

            strncpy(gantt_name[slots], c[i].name, 31);
            gantt_end[slots] = t;
            slots++;

            if (remaining[i] == 0) {
                done++;
                c[i].finish_time     = t;
                c[i].turnaround_time = c[i].finish_time - c[i].arrival_time;
                c[i].waiting_time    = c[i].turnaround_time - c[i].burst_time;
            }
        }
        if (!progressed) t++;
    }

    printf("\nGantt Chart:\n|");
    for (int s = 0; s < slots; s++)
        printf(" %-9s|", gantt_name[s]);
    printf("\n0");
    for (int s = 0; s < slots; s++)
        printf("%-11d", gantt_end[s]);
    printf("\n");

    print_metrics(c, n);
}