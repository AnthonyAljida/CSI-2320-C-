#include <stdio.h>
#include <unistd.h>

static int i;

int N = 0;

int main()
{

    for (i = 0; i < 2 + N; i++)
    {
        printf("A %d, %d, %d\n", i, getpid(), getppid());
        fflush(stdout);

        fork();

        printf("B %d, %d, %d\n", i, getpid(), getppid());
        fflush(stdout);
    }
}
