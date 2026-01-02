#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>


int main()
{
    for(int i = 0; i < 10; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return -1;
        }
        if (pid == 0)
        {
            // child process
            printf("i=%d, pid: %d\n", i, getpid());
            break;
        }
    }
    
    return 0;
}
