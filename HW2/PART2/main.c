#include <stdbool.h>
#include <signal.h>
#include "dinningPhilosopher.h"


const int NumberOfPhilosopher = 5;
bool running = false;

void stop();

int main()
{
    signal(SIGINT, stop);
    dinningPhilosopher *myDinningPhilosopher = initDinningPhilosopher(NumberOfPhilosopher);
    runDinningPhilosopher(myDinningPhilosopher);
    destroyDinningPhilosopher(myDinningPhilosopher);
    return 0;
}

void stop()
{
    running = false;
}
