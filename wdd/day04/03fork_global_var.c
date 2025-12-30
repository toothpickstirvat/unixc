#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int g = 1024;

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
        printf("[Child] g = %d, [%p]\n", g, &g);
        g = 9527;
        printf("[Child] g = %d, [%p]\n", g, &g);
    }
    else
    {
        getchar();
        printf("[Parent] g = %d, [%p]\n", g, &g);
    }
    
    for(;;);

    return 0;
}
