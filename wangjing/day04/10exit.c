#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

void before_exit1()
{
    printf("[before_exit1] process exit!\n");
}

void before_exit2(int num, void* arg)
{
    // num是进程退出的返回值
    // arg是on_exit的第二个参数
    printf("[before_exit2] %d, %s\n", num, (char*)arg);
}

int main()
{
    atexit(before_exit1);
    
    on_exit(before_exit2, "hello");

    printf("main end!\n");
    return 0;
}
