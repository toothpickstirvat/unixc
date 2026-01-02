#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

int main()
{
    int a = 1024;
    pid_t pid = vfork();
    if (pid == -1)
    {
        perror("vfork");
        return -1;
    }
    if (pid == 0)
    {
        printf("[Child] pid: %d parent: %d\n", getpid(), getppid());
        printf("[Child] a = %d\n", a);
        a = 9527;
        printf("[Child] a = %d\n", a);
        exit(0); // 不能用return
    }
    else
    {
        printf("[Parent] pid: %d\n", getpid());
        sleep(2);
        printf("[Parent] a = %d\n", a);
    }

    return 0;
}
