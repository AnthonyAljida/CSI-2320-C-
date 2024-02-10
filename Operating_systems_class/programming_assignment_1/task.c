#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t count = 0;

int main()
{
    // Set the signal handler to ignore SIGINT
    if (signal(SIGINT, SIG_IGN) == SIG_ERR)
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
