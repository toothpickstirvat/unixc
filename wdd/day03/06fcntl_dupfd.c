#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int fd = open("a.txt", O_RDONLY);
    if (fd == -1) 
    {
        perror("fcntl");
        return -1;
    }
    int fd1 = fcntl(fd, F_DUPFD, 100);
    if (fd1 == -1) 
    {
        perror("fcntl");
        return -1;
    }

    printf("fd = %d\n", fd);
    printf("fd1 = %d\n", fd1);


    close(fd1);
    close(fd);

    return 0;
}

