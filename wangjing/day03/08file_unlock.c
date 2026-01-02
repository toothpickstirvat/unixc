#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    int fd = 0;
    if (access("a.dat", F_OK)==0) {
        fd = open("a.dat", O_RDWR, 0644);
    }
    else
    {
        fd = open("a.dat", O_RDWR | O_CREAT, 0644);
    }
    int n = 0;
    write(fd, &n, 4);

    if (fd == -1)
    {
        perror("open");
        return -1;
    }

    int i;
    for (i = 0; i < 1000000; i++)
    {
        lseek(fd, 0, SEEK_SET);
        int num = 0;
        read(fd, &num, 4);
        ++num;
        lseek(fd, 0, SEEK_SET);
        write(fd, &num, 4);
    }

    lseek(fd, 0, SEEK_SET);
    int num = 0;
    read(fd, &num, 4);
    printf("num=%d\n", num);

    close(fd);

    return 0;
}
