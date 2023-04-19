#ifndef __DENNING_PHILOSOPHER_H__
#define __DENNING_PHILOSOPHER_H__

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define THINKING 0 /* philosopher is thinking */
#define HUNGRY 1 /* philosopher is trying to get forks */
#define EATING 2 /* philosopher is eating */

typedef pthread_mutex_t semaphore;
typedef struct dinningPhilosopher dinningPhilosopher;
typedef struct philosopher philosopher;

struct dinningPhilosopher
{
    int philosopherCount;
    semaphore shareMutex;               // shared mutex among all philosophers. only allow one user to try to get fork at a time
    philosopher **philosophers;
};

struct philosopher
{
    int id;         
    pthread_t threadId;
    int state;              // current state of philosopher
    semaphore mutex;        // each philosopher has his own mutex
    dinningPhilosopher *dinningPhilosopher; // be used to get share mutex and how many philosophers are at the table
};

dinningPhilosopher *initDinningPhilosopher(int n);
void destroyDinningPhilosopher(dinningPhilosopher *p);
void runDinningPhilosopher(dinningPhilosopher *dinningPhilosopher);

#endif