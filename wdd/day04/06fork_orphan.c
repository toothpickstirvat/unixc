#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return -1;
    }
    if (pid == 0)
    {
        printf("[Child] pid: %d, parent(%d)\n", getpid(), getppid());
        sleep(10);
        printf("[Child] pid: %d, parent(%d)\n", getpid(), getppid());
    }
    else
    {
        printf("[Parent] pid: %d has a child(%d)\n", getpid(), pid);
        sleep(1);
        printf("[Parent] pid: %d will die.\n", getpid());
    }

    return 0;
}