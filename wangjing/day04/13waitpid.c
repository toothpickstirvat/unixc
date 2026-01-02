#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

int main()
{
    srand(time(NULL));

    pid_t pids[10];
    for(int i = 0; i< 10; i++)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            return -1;
        }
        if (pids[i]==0)
        {
            int sec = 20 + i*i;
            printf("[Child] pid: %d, sleep: %d\n", getpid(), sec);
            sleep(sec);
            printf("[Child pid: %d exit, return value: %d\n", getpid(), i * i);
            return i * i;
        }
    }
    int num=0;
    while(1)
    {
        int status = 0;
        pid_t pid = waitpid(-1, &status, WNOHANG);
        if (pid == -1)
        {
            perror("waitpid");
            printf("num=%d\n", num);
            return -1;
        }
        else if (pid == 0)
        {
            ++num;
            continue;
        }

        if (WIFEXITED(status))
        {
            printf("child: %d normal exit, return value: %d\n", pid, WEXITSTATUS(status));
        }

        else if(WIFSIGNALED(status))
        {
            printf("child: %d terminated by signal: %d\n", pid, WTERMSIG(status));
        }
        else if(WIFSTOPPED(status))
        {
            printf("child: %d stopped, signal: %d\n", pid, SIGSTOP);
        }
        else if(WIFCONTINUED(status))
        {
            printf("child: %d continued, signal: %d\n", pid, SIGCONT);
        }
    }
    return 0;
}
