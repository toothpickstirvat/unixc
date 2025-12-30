#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int g = 1024;

int main()
{
    int* p = (int*)malloc(4);
    *p = 100;
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return -1;
    }
    if (pid == 0)
    {
        printf("[Child] p=%p, *p=%d\n", p, *p);
        *p = 9527;
        printf("[Child] p=%p, *p=%d\n", p, *p);
        free(p);
    }
    else
    {
        getchar();
        printf("[Parent] p=%p, *p=%d\n", p, *p);
        *p = 1314;
        printf("[Parent] p=%p, *p=%d\n", p, *p);
        free(p);
    }
    

    return 0;
}