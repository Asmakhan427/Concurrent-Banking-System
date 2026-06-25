# 🏦 Concurrent Banking System - Operating Systems Project

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Pthread](https://img.shields.io/badge/Pthreads-0078D6?style=for-the-badge&logo=linux&logoColor=white)

## 📌 Project Overview

A complete multi-threaded banking simulation that demonstrates **6 core Operating System concepts** working together in a real-world scenario. This project was developed for **CL2006 – Operating System Lab (Spring 2026)**.

### 🎯 Objectives

- Implement multithreading with different scheduling algorithms
- Demonstrate synchronization using mutex locks and semaphores
- Apply Banker's Algorithm for deadlock avoidance
- Show IPC using message queues and pipes
- Compare FIFO vs LRU page replacement algorithms
- Demonstrate priority-based preemption

---

## 👥 Customer Types

| Customer Type | Priority | Description |
|---------------|----------|-------------|
| Regular | 1 | Performs deposits/withdrawals |
| Premium | 3 | Higher scheduling priority |
| Loan Applicant | 2 | Banker's Algorithm verification |
| Corporate | 4 | Spawns 50 payroll threads |
| VIP | 5 | Highest priority with preemption |

---

## 🛠️ OS Concepts Implemented

### 1️⃣ Threading & Scheduling
- **FCFS** (First Come First Serve) - Non-preemptive, arrival order
- **Priority Scheduling** - VIP(5) > Corporate(4) > Premium(3) > Loan(2) > Regular(1)
- **Round Robin** - Time quantum = 2ms
- **Output:** Gantt charts + Waiting/Turnaround/Response times

### 2️⃣ Synchronization
- **Mutex Locks** - Protect shared account balances (critical sections)
- **Semaphores** - Limit concurrent deposits to max 2
- **Race Condition Prevention** - Demonstrated with/without mutex

### 3️⃣ Deadlock Handling (Banker's Algorithm)
- Safety algorithm checks if loan request keeps system safe
- Safe sequence identification
- Automatic rollback on unsafe state
- **Test cases:** Safe grant ✅ | Unsafe denial ❌

### 4️⃣ IPC (Inter-Process Communication)
- **Message Queues** (System V) - Producer-consumer pattern
- **Pipes** - Alternative communication method
- Customer processes send requests → Bank server processes replies

### 5️⃣ Memory Management
- **FIFO** (First-In-First-Out) page replacement
- **LRU** (Least Recently Used) page replacement
- Page fault and hit ratio comparison
- Each customer has simulated memory footprint

### 6️⃣ Priority Preemption
- VIP customer arrives during Regular customer execution
- VIP preempts immediately (higher priority)
- Regular resumes after VIP completes


