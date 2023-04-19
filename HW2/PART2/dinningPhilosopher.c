#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "dinningPhilosopher.h"


static philosopher *initPhilosopher(int id, int state, dinningPhilosopher* dinningPhilosopher);
static void destroyPhilosopher(philosopher *philosopher);

static philosopher *move(philosopher *philosopherInfo, int step);
static void *philosopherStart(void *philosopherInfo);
static void takeForks(philosopher* philosopherInfo);
static void putForks(philosopher* philosopherInfo);
static void test(philosopher* philosopherInfo);
static void think();
static void eat();

extern bool running;

dinningPhilosopher *initDinningPhilosopher(int n)
{
    dinningPhilosopher *myDinningPhilosopher = malloc(sizeof(dinningPhilosopher));
    myDinningPhilosopher -> philosophers = calloc(n, sizeof(philosopher*));
    myDinningPhilosopher -> philosopherCount = n;
    for(int i = 0; i < n; ++i)
        myDinningPhilosopher -> philosophers[i] = initPhilosopher(i, THINKING, myDinningPhilosopher);

    pthread_mutex_init(&myDinningPhilosopher->shareMutex, NULL);
    return myDinningPhilosopher;
}

void runDinningPhilosopher(dinningPhilosopher *dinningPhilosopher)
{

    running = true;
    // every philosopher run their own thread
    for(int i = 0 ; i < dinningPhilosopher->philosopherCount ; ++i)
	{
		int ret = pthread_create(&dinningPhilosopher->philosophers[i]->threadId, NULL, &philosopherStart, (void*)dinningPhilosopher->philosophers[i]);
        if(ret != 0)
            handle_error_en(ret, "pthread_create");
	}

    int count = 1;
	while(running)
	{
		pthread_mutex_lock(&dinningPhilosopher->shareMutex);
		printf("======%d======\n",count);
		for(int i = 0 ; i < dinningPhilosopher->philosopherCount ; ++i)
		{
			if(dinningPhilosopher->philosophers[i]->state == THINKING)
                printf("%d : THINKING\n", i+1);
            else if(dinningPhilosopher->philosophers[i]->state == HUNGRY)
				printf("%d : HUNGRY\n", i+1);
			else
				printf("%d : EATING\n", i+1);
		}
		pthread_mutex_unlock(&dinningPhilosopher->shareMutex);
		sleep(1);
		++count;
	}

    for(int i = 0 ; i < dinningPhilosopher->philosopherCount; ++i)
	{
		int ret = pthread_join(dinningPhilosopher->philosophers[i]->threadId, NULL);
        if(ret != 0)
            handle_error_en(ret, "pthread_join");
	}
}

void destroyDinningPhilosopher(dinningPhilosopher *p)
{
    pthread_mutex_destroy(&p->shareMutex);
    for(int i = 0; i < p->philosopherCount; ++i)
        destroyPhilosopher(p->philosophers[i]);
    free(p->philosophers);
    free(p);
}

static philosopher *initPhilosopher(int id, int state, dinningPhilosopher* dinningPhilosopher)
{
    philosopher *currentPhilosopher = malloc(sizeof(philosopher));
    currentPhilosopher -> id = id;
    currentPhilosopher -> state = state;
    currentPhilosopher -> dinningPhilosopher = dinningPhilosopher;
    pthread_mutex_init(&currentPhilosopher->mutex, NULL);
    pthread_mutex_lock(&currentPhilosopher->mutex);
    return currentPhilosopher;
}

static void destroyPhilosopher(philosopher *philosopher)
{
    pthread_mutex_destroy(&philosopher->mutex);
    free(philosopher);
}

static philosopher *move(philosopher *philosopherInfo, int step)
{
    int count = philosopherInfo -> dinningPhilosopher -> philosopherCount;
    int currentId = philosopherInfo -> id;
    int id = (currentId + count + step) % count;
    return philosopherInfo->dinningPhilosopher->philosophers[id];
}

static void *philosopherStart(void *philosopherInfo) /* i: philosopher number, from 0 to N−1 */
{
    while (running) { /* repeat forever */
        think(); /* philosopher is thinking */
        takeForks((philosopher*)philosopherInfo); /* acquire two forks or block */
        eat(); /* yum-yum, spaghetti */
        putForks((philosopher*)philosopherInfo); /* put both forks back on table */
    }

    printf("philosopher %d stop!\n", ((philosopher*)philosopherInfo)->id+1);

    return NULL;
}

static void takeForks(philosopher* philosopherInfo) /* i: philosopher number, from 0 to N−1 */
{
    pthread_mutex_lock(&philosopherInfo->dinningPhilosopher->shareMutex); /* enter critical region */
    philosopherInfo->state = HUNGRY; /* record fact that philosopher i is hungry */
    test(philosopherInfo); /* try to acquire 2 forks */
    pthread_mutex_unlock(&philosopherInfo->dinningPhilosopher->shareMutex); /* exit critical region */
    pthread_mutex_lock(&philosopherInfo->mutex); /* block if forks were not acquired */
}

static void putForks(philosopher* philosopherInfo) /* i: philosopher number, from 0 to N−1 */
{
    pthread_mutex_lock(&philosopherInfo->dinningPhilosopher->shareMutex); /* enter critical region */
    philosopherInfo->state = THINKING; /* philosopher has finished eating */
    test(move(philosopherInfo, -1)); /* see if left neighbor can now eat */
    test(move(philosopherInfo, 1)); /* see if right neighbor can now eat */
    pthread_mutex_unlock(&philosopherInfo->dinningPhilosopher->shareMutex); /* exit critical region */
}

static void test(philosopher* philosopherInfo) /* i: philosopher number, from 0 to N−1 */
{
    if (philosopherInfo->state == HUNGRY && move(philosopherInfo, -1)->state != EATING && move(philosopherInfo, 1)->state != EATING) {
        philosopherInfo->state = EATING;
        pthread_mutex_unlock(&philosopherInfo->mutex);
    }
}

static void think()
{
    sleep(1);
}

static void eat()
{
    sleep(1);
}