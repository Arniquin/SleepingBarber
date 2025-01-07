// Arnold Richards Alvarez 201866077
// Sleeping Barber Problem Implementation

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

// Define semaphore indices and waiting room capacity
#define CLIENTES 0  // Semaphore for waiting clients
#define BARBERO 1   // Semaphore for barber availability
#define MUTEX 2     // Semaphore for critical section (mutual exclusion)
#define ASIENTOS 5  // Number of waiting room seats

// Function to increment a semaphore (signal operation)
void down(int semId, int semNum, struct sembuf *buf) {
    buf->sem_num = semNum;  // Specify which semaphore to operate on
    buf->sem_op = 1;        // Increment the semaphore value
    buf->sem_flg = 0;       // No flags
    semop(semId, buf, 1);   // Perform the semaphore operation
}

// Function to decrement a semaphore (wait operation)
void up(int semId, int semNum, struct sembuf *buf) {
    buf->sem_num = semNum;  // Specify which semaphore to operate on
    buf->sem_op = -1;       // Decrement the semaphore value
    buf->sem_flg = 0;       // No flags
    semop(semId, buf, 1);   // Perform the semaphore operation
}

// Function to initialize a semaphore with a given value
void initSem(int semId, int semNum, int val) {
    union semnum {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } argument;
    argument.val = val;  // Set the initial value of the semaphore
    semctl(semId, semNum, SETVAL, argument);  // Initialize the semaphore
}

// Barber process logic
// The barber will wait for clients and serve them when available
void barbero(int semId, int *espera) {
    struct sembuf buf;  // Semaphore buffer for operations
    while (1) {
        up(semId, CLIENTES, &buf);  // Wait for a client to arrive
        up(semId, MUTEX, &buf);     // Enter critical section to modify shared variable
        *espera = *espera - 1;      // Decrease the number of waiting clients
        down(semId, BARBERO, &buf); // Signal that the barber is available
        down(semId, MUTEX, &buf);   // Exit critical section
        printf("The barber is attending a client...\n");
        sleep(6);  // Simulate the time taken to cut hair
    }
}

// Client process logic
// Each client will try to get a seat and wait if the barber is busy
void cliente(int semId, int *espera) {
    int clientes = 1;              // Client identifier counter
    struct sembuf semaphore;       // Semaphore buffer for operations
    while (1) {
        sleep(3);  // Simulate the arrival time of a new client
        down(semId, MUTEX, &semaphore);  // Enter critical section to check waiting room
        if (*espera < ASIENTOS) {   // If there are available seats
            printf("Client %d is waiting in a seat\n", clientes++);
            *espera = *espera + 1;  // Increase the number of waiting clients
            up(semId, CLIENTES, &semaphore);  // Signal a client is ready
            up(semId, MUTEX, &semaphore);     // Exit critical section
        } else {  // No seats available
            printf("Client %d left due to no available seats\n", clientes++);
            up(semId, MUTEX, &semaphore);  // Exit critical section
        }
    }
}

// Main function - sets up shared memory and semaphores, and creates barber and client processes
int main() {
    int semKey = 1111, shmKey = 2222;  // Keys for semaphore and shared memory
    int semId, shmId;  // Semaphore and shared memory identifiers
    int *espera;       // Pointer to shared memory for tracking waiting clients

    // Create shared memory segment to track the number of waiting clients
    shmId = shmget(shmKey, sizeof(int), IPC_CREAT | 0666);
    // Create semaphore set with 3 semaphores (CLIENTES, BARBERO, MUTEX)
    semId = semget(semKey, 3, IPC_CREAT | 0666);
    // Attach the shared memory segment to the process
    espera = shmat(shmId, NULL, 0);
    *espera = 0;  // Initialize the number of waiting clients to 0

    // Initialize the semaphores: 0 clients, 0 barber available, 1 for mutex (critical section)
    initSem(semId, CLIENTES, 0);
    initSem(semId, BARBERO, 0);
    initSem(semId, MUTEX, 1);

    printf("There are %d waiting room seats\n", ASIENTOS);

    // Create barber and client processes using fork
    if (fork()) {
        // Parent process: Barber
        barbero(semId, espera);
    } else {
        // Child process: Client
        cliente(semId, espera);
    }
}
