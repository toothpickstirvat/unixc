#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

void sigint_handler(int signum)
{
    printf("%d received %d\n", getpid(), signum);
}

int main()
{
    printf("pid=%d\n", getpid());
    // 注册信号处理函数
    // 进程收到信号，中止现在的任务去执行信号处理函数
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
    {
        perror("signal");
        return -1;
    }
    if(signal(SIGQUIT, SIG_IGN) == SIG_ERR)
    {
        perror("signal");
    }
    if(signal(SIGKILL, SIG_IGN) == SIG_ERR)
    {
        printf("9 cannot be captured and ignored\n");
    }
    
    while(1);

    return 0;
}

