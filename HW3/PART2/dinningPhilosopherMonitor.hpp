#include <pthread.h>
#include <vector>

class Philosopher_monitor {
public:
    enum class status {
        THINKING,
        HUNGRY,
        EATING
    };
    Philosopher_monitor(int num);
    ~Philosopher_monitor();
    static Philosopher_monitor *getInstance(int num);
    void run();
    static void sig_terminate(int sig);

private:
    static Philosopher_monitor *instance;
    bool running;
    int philosopher_num;
    std::vector<pthread_t> philosophers_id;
    std::vector<pthread_cond_t> philosophers_cv;
    pthread_mutex_t share_lock;
    std::vector<status> philosophers_state;
    void take_fork(int who);
    void put_fork(int who);
    int test(int who);
    void think();
    void eat();
    void output();
    void start(int args);
    static void *thread_start(void* args);
    struct Thread_args {
        Philosopher_monitor* This;
        int philosopher_num;
        Thread_args(Philosopher_monitor* _THIS, int _philosopher_num)
            : This(_THIS), philosopher_num(_philosopher_num) {}
    };
};