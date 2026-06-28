# Concurrent Banking System

## Overview

The **Concurrent Banking System** is a multi-threaded banking simulation developed in **C** for the **Operating Systems Lab**. The project demonstrates the practical implementation of fundamental operating system concepts, including multithreading, synchronization, deadlock avoidance, inter-process communication (IPC), memory management, and process scheduling in a realistic banking environment.

---

## Features

* Multi-threaded customer transaction processing
* Multiple CPU scheduling algorithms
* Synchronization using mutexes and semaphores
* Deadlock avoidance using Banker's Algorithm

---

## Technologies Used

* C Programming
* POSIX Threads (Pthreads)
* Linux
* System V Message Queues
* Pipes
* Mutex Locks
* Semaphores

---

## Core Concepts Implemented

### Thread Scheduling

The system supports multiple CPU scheduling algorithms, including **First Come First Serve (FCFS)**, **Priority Scheduling**, and **Round Robin**, to execute customer requests efficiently.

<img width="814" height="526" alt="image" src="https://github.com/user-attachments/assets/acc2faeb-ce2f-47df-8dc8-ca5880154fcd" />


---

### Synchronization

Shared bank account data is protected using **mutex locks**, while **semaphores** regulate concurrent access to critical banking operations, preventing race conditions.

<img width="950" height="520" alt="image" src="https://github.com/user-attachments/assets/e75edc07-171b-40e5-9f1b-33866d62b12b" />


---

### Deadlock Avoidance

The **Banker's Algorithm** verifies whether loan requests maintain a safe system state before allocating resources, preventing deadlocks.

<img width="874" height="334" alt="image" src="https://github.com/user-attachments/assets/21977026-f504-47ec-a3ff-b9b141f18841" />


---

### Inter-Process Communication (IPC)

Customer requests are exchanged between processes using **System V Message Queues** and **Pipes**, demonstrating communication between independent processes.

<img width="819" height="524" alt="image" src="https://github.com/user-attachments/assets/c0e2d09e-d029-4a28-afd1-72b6ca4e01e9" />


---

### Memory Management

The project simulates **FIFO** and **Least Recently Used (LRU)** page replacement algorithms and compares their page fault performance.

<img width="635" height="523" alt="image" src="https://github.com/user-attachments/assets/69d6dc0a-dd76-437a-ac18-bf724985988e" />


---

### Priority Preemption

High-priority customers can preempt currently executing lower-priority processes, demonstrating priority-based scheduling behavior.

<img width="874" height="436" alt="image" src="https://github.com/user-attachments/assets/822ce3f5-d997-4fc4-9059-9ae94e6b0cf4" />


---

## Project Structure

```text
Concurrent-Banking-System/
│
├── src/
├── include/
├── screenshots/
├── Makefile
├── README.md
└── bank_simulation.c
```

---

## How to Run

1. Clone the repository.

```bash
git clone https://github.com/Asmakhan427/Concurrent-Banking-System.git
```

2. Navigate to the project directory.

```bash
cd Concurrent-Banking-System
```

3. Compile the program.

```bash
gcc *.c -o banking_system -lpthread
```

4. Run the executable.

```bash
./banking_system
```

---

## Learning Outcomes

This project strengthened my understanding of:

* Multithreading with POSIX Threads
* CPU Scheduling Algorithms
* Process Synchronization
* Deadlock Avoidance
* Inter-Process Communication
* Memory Management
* Concurrent Programming
* Operating System Design

