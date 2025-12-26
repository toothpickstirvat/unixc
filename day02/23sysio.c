#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


int main() 
{
    int fd = open("sys.dat", O_WRONLY|O_CREAT|O_TRUNC);
    if (fd == -1) 
    {   
        perror("open");
        return -1;
    }

    for (int i = 0; i < 10000000; i++) 
    {
        // 每次调用都会从用户态切换到内核态
        write(fd, &i, sizeof(i));
    }

    close(fd);

    return 0;
}
