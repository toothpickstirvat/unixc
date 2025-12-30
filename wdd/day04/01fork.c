#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main()
{
    printf("main: %d, parent: %d\n", getpid(), getppid());
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return -1;
    }
    if (pid == 0)
    {
        // child process
        printf("[CHILD] pid(%d) created by parent(%d)\n", getpid(), getppid());
    }
    else
    {
        // parent process
        printf("[Parent] pid(%d) create child(%d)\n", getpid(), pid);
        getchar();
    }


    return 0;
}
