#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
    int fd = open("a.txt", O_CREAT|O_RDWR|O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        // child process
        char buf[] = "hello, world!";
        write(fd, buf, strlen(buf));
        printf("%ld\n", lseek(fd, 0, SEEK_CUR));
        close(fd);
    }
    else
    {
        // parent
        getchar();
        printf("%ld\n", lseek(fd, 0, SEEK_CUR));
        close(fd);
    }

    return 0;
}