# Sleeping Barber Problem Implementation

## Author
Arnold Richards Alvarez

## Overview
This project implements the **Sleeping Barber Problem**, a classic synchronization problem, using C. The problem models a barber shop with a barber, a waiting room with a fixed number of seats, and clients arriving at random intervals. The barber sleeps when there are no clients and serves them in the order they arrive when they are present.

## Features
- **Semaphore-based Synchronization**: Ensures mutual exclusion and proper synchronization between barber and client processes.
- **Shared Memory**: Tracks the number of clients waiting in the barber shop.
- **Concurrency**: Simulates real-world behavior with multiple clients attempting to enter the barber shop.

## Problem Explanation
1. **Barber**:
   - Sleeps when no clients are present.
   - Wakes up and serves a client when one arrives.

2. **Clients**:
   - Arrive at random intervals.
   - Take a seat if available or leave if the waiting room is full.

3. **Waiting Room**:
   - Has a fixed number of seats (`ASIENTOS`).
   - Clients wait here for their turn if the barber is busy.

## Implementation Details
### Key Components:
1. **Semaphores**:
   - `CLIENTES`: Tracks waiting clients.
   - `BARBERO`: Indicates when the barber is available.
   - `MUTEX`: Ensures mutual exclusion when modifying shared data.

2. **Shared Memory**:
   - Stores the count of waiting clients.

3. **Processes**:
   - **Barber Process**: Sleeps until clients arrive and serves them one by one.
   - **Client Process**: Arrives at intervals, waits for service if a seat is available, or leaves if the shop is full.

### Code Highlights:
- **Critical Section Management**: Prevents race conditions using `MUTEX`.
- **Random Arrival and Service Times**: Simulates real-world behavior.
- **Concurrent Execution**: Uses `fork()` to create independent barber and client processes.

## How to Compile and Run
### Requirements:
- GCC compiler
- Linux environment (for IPC features)

### Steps:
1. Compile the code:
   ```bash
   gcc -o barber barber.c
