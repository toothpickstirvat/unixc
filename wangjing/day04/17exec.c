#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;
int main()
{
    pid_t pid = vfork();
    if (pid == -1)
    {
        perror("vfork");
        return -1;
    }

    if (pid == 0)
    {
        //execl("myproc", "aaa", "bbb", "123", NULL);

        char* env[] = {"path=.", "language=utf-8", NULL};
        // 指定环境变量
        //execle("myproc", "aaa", "bbb", "123", NULL, env);
        char* args[] = {"myproc", "aaa", "bbb", "123", NULL};
        //execv("myproc", args);
        //execvpe("myproc", args, env);
        execve("myproc", args, env);
    }

    return 0;
}
