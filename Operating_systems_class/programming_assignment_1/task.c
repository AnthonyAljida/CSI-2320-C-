#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t count = 0;
volatile sig_atomic_t running = 1;

void handle_sigint(int sig)
{
    printf("Ignoring Ctrl+C (SIGINT) signal\n");
}

int main()
{
    // Set the signal handler to ignore SIGINT
    if (signal(SIGINT, SIG_IGN) == SIG_ERR)
    {
        perror("signal");
        return 1;
    }

    // Register a custom handler for SIGINT (just to demonstrate)
    if (signal(SIGINT, handle_sigint) == SIG_ERR)
    {
        perror("signal");
        return 1;
    }

    int mypid = getpid();

    while (1)
    {
        printf("Process %d at iteration %d\n", mypid, count++);
        sleep(1); // You could also use usleep() with specified high resolution time length
    }

    return 0;
}
