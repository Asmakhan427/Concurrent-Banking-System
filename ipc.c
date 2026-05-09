#include "banking.h"

typedef struct {
    long mtype;
    char mtext[MSG_SIZE];
} Message;

void run_ipc_demo(void)
{
    printf("\n==========  MODULE 4a : IPC - MESSAGE QUEUE  ==========\n\n");

    key_t key   = ftok("banking.h", 65);
    int   msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid < 0) { perror("msgget"); return; }

    struct {
        char   name[24];
        char   op[12];
        int    account;
        double amount;
    } txns[] = {
        {"Abdullah", "Deposit",  101,  500.0},
        {"Khadija",  "Withdraw", 102,  200.0},
        {"Ibrahim",  "Deposit",  103, 1500.0},
        {"Maryam",   "Withdraw", 104,  300.0},
        {"Yusuf",    "Deposit",  105, 2000.0},
    };
    int ntx = sizeof(txns) / sizeof(txns[0]);

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return; }

    if (pid == 0) {
        Message msg;
        msg.mtype = 1;
        for (int i = 0; i < ntx; i++) {
            snprintf(msg.mtext, MSG_SIZE, "REQ|%s|%s|%d|%.2f",
                     txns[i].name, txns[i].op,
                     txns[i].account, txns[i].amount);
            msgsnd(msgid, &msg, sizeof(msg.mtext), 0);
            printf("[CUSTOMER] %-10s sent:  %s $%.2f to Account %d\n",
                   txns[i].name, txns[i].op,
                   txns[i].amount, txns[i].account);

            Message resp;
            msgrcv(msgid, &resp, sizeof(resp.mtext), 2, 0);
            printf("[CUSTOMER] %-10s recv:  %s\n\n",
                   txns[i].name, resp.mtext);
            usleep(100000);
        }
        snprintf(msg.mtext, MSG_SIZE, "DONE");
        msgsnd(msgid, &msg, sizeof(msg.mtext), 0);
        exit(0);
    }

    printf("[SERVER]  Bank server PID=%d ready.\n\n", getpid());

    while (1) {
        Message req;
        msgrcv(msgid, &req, sizeof(req.mtext), 1, 0);
        if (strcmp(req.mtext, "DONE") == 0) break;

        char   name[24], op[12];
        int    account;
        double amount;
        sscanf(req.mtext, "REQ|%23[^|]|%11[^|]|%d|%lf",
               name, op, &account, &amount);

        printf("[SERVER]  Processing: %-10s %s $%.2f on Account %d\n",
               name, op, amount, account);

        int rc = (strcmp(op, "Deposit") == 0)
                 ? deposit(account, amount, name)
                 : withdraw(account, amount, name);

        Message resp;
        resp.mtype = 2;
        snprintf(resp.mtext, MSG_SIZE,
                 rc == 0 ? "Transaction Successful" : "Transaction Failed");
        msgsnd(msgid, &resp, sizeof(resp.mtext), 0);
    }

    wait(NULL);
    msgctl(msgid, IPC_RMID, NULL);
    printf("\n[SERVER]  IPC complete. Message queue removed.\n");
    print_accounts();
}

void run_pipe_demo(void)
{
    printf("\n==========  MODULE 4b : IPC - PIPES  ==========\n\n");

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        close(pipefd[PIPE_READ]);
        
        struct {
            char name[24];
            char op[12];
            int account;
            double amount;
        } requests[] = {
            {"Hamza",  "Deposit", 101, 100.0},
            {"Amina",  "Withdraw", 102, 50.0},
            {"Hassan", "Deposit", 103, 200.0},
        };
        
        for (int i = 0; i < 3; i++) {
            char buffer[MSG_SIZE];
            snprintf(buffer, MSG_SIZE, "%s|%s|%d|%.2f",
                     requests[i].name, requests[i].op,
                     requests[i].account, requests[i].amount);
            write(pipefd[PIPE_WRITE], buffer, strlen(buffer) + 1);
            printf("[PIPE-CUSTOMER] Sent: %s\n", buffer);
            usleep(50000);
        }
        
        write(pipefd[PIPE_WRITE], "DONE", 5);
        close(pipefd[PIPE_WRITE]);
        exit(0);
    }
    
    if (pid > 0) {
        close(pipefd[PIPE_WRITE]);
        
        char buffer[MSG_SIZE];
        printf("[PIPE-SERVER] Bank server ready (PID=%d)\n", getpid());
        
        while (1) {
            ssize_t bytes = read(pipefd[PIPE_READ], buffer, sizeof(buffer));
            if (bytes <= 0) break;
            if (strcmp(buffer, "DONE") == 0) break;
            
            char name[24], op[12];
            int account;
            double amount;
            sscanf(buffer, "%23[^|]|%11[^|]|%d|%lf",
                   name, op, &account, &amount);
            
            printf("[PIPE-SERVER] Processing: %s %s $%.2f on Account %d\n",
                   name, op, amount, account);
            
            if (strcmp(op, "Deposit") == 0)
                deposit(account, amount, name);
            else
                withdraw(account, amount, name);
        }
        
        close(pipefd[PIPE_READ]);
        wait(NULL);
        printf("\n[PIPE-SERVER] Pipe communication complete.\n");
        print_accounts();
    }
}