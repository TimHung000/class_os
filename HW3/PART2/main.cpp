#include <csignal>
#include "dinningPhilosopherMonitor.hpp"

Philosopher_monitor *Philosopher_monitor::instance = nullptr;

int main() {
    const int philosopher_num = 5;
    Philosopher_monitor *myPhilosopher = Philosopher_monitor::getInstance(philosopher_num);
    myPhilosopher->run();

    return 0;
}

