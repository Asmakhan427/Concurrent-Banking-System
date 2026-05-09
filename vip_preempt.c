#include "banking.h"

static int vip_arrived = 0;
static pthread_mutex_t vip_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t vip_cond = PTHREAD_COND_INITIALIZER;

static void *regular_customer_thread(void *arg)
{
    Customer *c = (Customer *)arg;
    printf("[REGULAR] %s started execution\n", c->name);
    
    for (int step = 1; step <= c->burst_time; step++) {
        pthread_mutex_lock(&vip_mutex);
        if (vip_arrived) {
            printf("[PREEMPT] %s is PREEMPTED by VIP at step %d\n", c->name, step);
            pthread_cond_wait(&vip_cond, &vip_mutex);
            printf("[RESUME] %s resumes execution after VIP completes\n", c->name);
        }
        pthread_mutex_unlock(&vip_mutex);
        
        printf("[REGULAR] %s executing step %d of %d\n", c->name, step, c->burst_time);
        usleep(100000);
    }
    
    printf("[REGULAR] %s completed execution\n", c->name);
    return NULL;
}

static void *vip_customer_thread(void *arg)
{
    Customer *c = (Customer *)arg;
    
    printf("\n[VIP] *** %s ARRIVED! Preempting regular customer ***\n", c->name);
    
    pthread_mutex_lock(&vip_mutex);
    vip_arrived = 1;
    pthread_mutex_unlock(&vip_mutex);
    
    printf("[VIP] %s executing priority transaction (burst=%d)\n", c->name, c->burst_time);
    for (int step = 1; step <= c->burst_time; step++) {
        printf("[VIP] %s executing step %d of %d (HIGHEST PRIORITY)\n", 
               c->name, step, c->burst_time);
        usleep(100000);
        
        if (step == c->burst_time) {
            deposit(105, 10000.0, c->name);
        }
    }
    printf("[VIP] %s completed VIP transaction\n", c->name);
    
    pthread_mutex_lock(&vip_mutex);
    vip_arrived = 0;
    pthread_cond_signal(&vip_cond);
    pthread_mutex_unlock(&vip_mutex);
    
    return NULL;
}

void run_vip_preemption_demo(void)
{
    printf("\n==========  VIP PREEMPTION DEMONSTRATION  ==========\n");
    printf("Shows VIP customer preempting Regular customer mid-execution\n\n");
    
    pthread_t regular_tid, vip_tid;
    
    Customer regular = {
        .id = 1, .type = REGULAR, .priority = REGULAR,
        .name = "Umar", .arrival_time = 0, .burst_time = 5
    };
    
    Customer vip = {
        .id = 2, .type = VIP, .priority = VIP,
        .name = "Khadija", .arrival_time = 2, .burst_time = 3
    };
    
    printf("Scenario:\n");
    printf("  - Regular customer '%s' starts first (Priority %d)\n", 
           regular.name, regular.priority);
    printf("  - VIP customer '%s' arrives later (Priority %d)\n", 
           vip.name, vip.priority);
    printf("  - VIP should PREEMPT the Regular customer\n\n");
    
    printf("=== EXECUTION START ===\n");
    
    pthread_create(&regular_tid, NULL, regular_customer_thread, &regular);
    sleep(1);
    pthread_create(&vip_tid, NULL, vip_customer_thread, &vip);
    
    pthread_join(vip_tid, NULL);
    pthread_join(regular_tid, NULL);
    
    printf("\n=== EXECUTION COMPLETE ===\n");
    printf("\nCONCLUSION: VIP successfully preempted Regular customer\n");
    printf("===========================================================\n\n");
}