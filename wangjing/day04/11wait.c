#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
    for (int i = 0; i < 10; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return -1;
        }
        if (pid == 0)
        {
            sleep(i+10);
            printf("[Child] pid: %d\n", getpid());
            return i*i;
        }
    }
    for (int i = 0; i < 12; i++)
    {
        int status = 0;
        pid_t pid = wait(&status); // 没有子进程结束则阻塞
        if (pid == -1)
        {
            if (errno == ECHILD)
            {
                printf("no child process to wait\n");
            }
            else
            {
                perror("wait");
            }
        }
        //printf("child: %d exit, return value: %d\n", pid, (status >> 8) & 0xFF);
        printf("child: %d exit, return value: %d\n", pid, WEXITSTATUS(status));
    }

    return 0;
}



