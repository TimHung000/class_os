#include "dinningPhilosopherMonitor.hpp"
#include <pthread.h>
#include <iostream>
#include <cstdlib>        // exit
#include <unistd.h>       // sleep
#include <csignal>


Philosopher_monitor::Philosopher_monitor(int num) {
    philosopher_num = num;
    running = false;
    philosophers_cv.resize(num);
    philosophers_id.resize(num);
    if(pthread_mutex_init(&share_lock, NULL) != 0) {
        std::cerr << "failed to init mutex lock!\n";
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < philosopher_num; ++i) {
        if(pthread_cond_init(&philosophers_cv[i], NULL) != 0 ) {
            std::cerr << "failed to init conditional variable\n";
            exit(EXIT_FAILURE);
        }
        philosophers_state.push_back(status::THINKING);
    }
}

Philosopher_monitor::~Philosopher_monitor() {
    if(pthread_mutex_destroy(&share_lock) != 0) {
        std::cerr << "failed to destory mutex lock!\n";
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < philosopher_num; ++i) {
        if(pthread_cond_destroy(&philosophers_cv[i]) != 0 ) {
            std::cerr << "failed to destroy conditional variable\n";
            exit(EXIT_FAILURE);
        }
    }
}

Philosopher_monitor *Philosopher_monitor::getInstance(int num) {
    if(!instance) {
        instance = new Philosopher_monitor(num);
    }
    return instance;
}

void Philosopher_monitor::run() {
    std::signal(SIGINT, Philosopher_monitor::sig_terminate);
    running = true;
    for(int i = 0 ; i < philosopher_num ; ++i)
	{
		if(pthread_create(&philosophers_id[i], NULL, &Philosopher_monitor::thread_start, (void*)new Thread_args(this, i)) != 0 ) {
            std::cerr << "failed to create pthread\n";
            exit(EXIT_FAILURE);
        }
	}

    while(running);

    for(int i = 0 ; i < philosopher_num; ++i)
	{
		if(pthread_join(philosophers_id[i], NULL) != 0) {
            perror("failed to create pthread");
            exit(EXIT_FAILURE);
        }
	}
}

void Philosopher_monitor::sig_terminate(int sig) {
    if(sig == SIGINT)
        instance->running = false;
}

void *Philosopher_monitor::thread_start(void* args) {
    Thread_args* myArgs = static_cast<Thread_args*>(args);
    myArgs->This->start(myArgs->philosopher_num);
    delete myArgs;
    return NULL;
}

void Philosopher_monitor::start(int who) {
    while (running) { /* repeat forever */
        think(); /* philosopher is thinking */
        take_fork(who); /* acquire two forks or block */
        eat(); /* yum-yum, spaghetti */
        put_fork(who); /* put both forks back on table */
    }
    std::cout << "philosopher " << who+1 << " stop!\n";
}

void Philosopher_monitor::take_fork(int who) {
    pthread_mutex_lock(&share_lock); /* enter critical region */
    philosophers_state[who] = status::HUNGRY; /* record fact that philosopher i is hungry */
    output();
    if(test(who) == 0) /* try to acquire 2 forks */
        pthread_cond_wait(&philosophers_cv[who], &share_lock);
    output();
    pthread_mutex_unlock(&share_lock); /* exit critical region */
}

void Philosopher_monitor::put_fork(int who) {
    pthread_mutex_lock(&share_lock); /* enter critical region */
    philosophers_state[who] = status::THINKING; /* philosopher has finished eating */
    output();
    test((who+philosopher_num-1) % philosopher_num); /* see if left neighbor can now eat */
    test((who+1) % philosopher_num); /* see if right neighbor can now eat */
    pthread_mutex_unlock(&share_lock); /* exit critical region */
}

int Philosopher_monitor::test(int who) {
    if (philosophers_state[who] == status::HUNGRY && philosophers_state[(who+philosopher_num-1) % philosopher_num] != status::EATING && philosophers_state[(who+1) % philosopher_num] != status::EATING) {
        philosophers_state[who] = status::EATING;
        pthread_cond_signal(&philosophers_cv[who]);
        return 1;
    }
    return 0;
}

void Philosopher_monitor::output() {
    static int count = 0;
    ++count;
    std::cout << "======" << count << "======\n";
    for(int i = 0 ; i < philosopher_num ; ++i)
    {
        if(philosophers_state[i] == status::THINKING)
            std::cout << i+1 << "THINKING\n";
        else if(philosophers_state[i] == status::HUNGRY)
            std::cout << i+1 << "HUNGRY\n";
        else
            std::cout << i+1 << "EATING\n";
    }
}

void Philosopher_monitor::think()
{
    sleep(1);
}

void Philosopher_monitor::eat()
{
    sleep(1);
}