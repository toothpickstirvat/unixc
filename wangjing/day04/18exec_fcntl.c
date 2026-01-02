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
    pid_t pid = vfork();
    if (pid == -1)
    {
        perror("vfork");
        return -1;
    }
    if (pid == 0)
    {
        int fd1 = open("b.txt", O_RDWR|O_CREAT|O_TRUNC, 0644);
        int fd2 = open("b.txt", O_RDWR|O_CREAT|O_TRUNC, 0644);
        if (fd1 == -1 || fd2 == -1)
        {
            perror("open");
            return -1;
        }
        // 在execve所替换的进程中,fd保持打开
        int ret = fcntl(fd1, F_SETFD, 0);
        if (ret == -1)
        {
            perror("fcntl");
        }
        // 在execve所替换的进程中,fd保持关闭
        ret = fcntl(fd2, F_SETFD, 1);
        if (ret == -1)
        {
            perror("fcntl");
        }

        char* argvs[] = {"myproc", "aaa", "bbb", "123", NULL};
        char* env[] = {"path=.", NULL};
        execve("myproc", argvs, env);
    }

    return 0;
}
