#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t count = 0;
volatile sig_atomic_t running = 1;

void my_sig_handler(int signum)
{
    running = 0;
}

int main(int argc, char *argv[])
{
    // Initialization and other necessary operations

    // Signal handler registration
    signal(SIGINT, my_sig_handler);

    int mypid = getpid();

    while (running)
    {
        printf("Process %d at iteration %d\n", mypid, count++);
        sleep(1); // You could also use usleep() with specified high resolution time length
    }

    return 0;
}
