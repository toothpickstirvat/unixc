#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

void func(int signum)
{
    printf("func!\n");
}

int main()
{
    func(10);
    printf("%d\n", getpid());
    
    // 收到信号2，去执行func函数，执行完后，继续执行循环
    signal(SIGINT, func);

    while(1);

    return 0;
}




