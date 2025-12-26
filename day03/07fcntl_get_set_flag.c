#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


void showflag(int fd) 
{
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1)
    {
        perror("fcntl");
        return;
    }

    struct{
        int flag;
        char str[48];
    } arr[5] = {
        {O_RDONLY, "O_RDONLY"},
        {O_WRONLY, "O_WRONLY"},
        {O_RDWR, "O_RDWR"},
        {O_APPEND, "O_APPEND"},
        {O_NONBLOCK, "O_NONBLOCK"}
    };
    for (int i=0; i < 5; i++)
    {
        if (flags & arr[i].flag)
        {
            printf("%s ", arr[i].str);
        }
    }
    printf("\n");
}

void addflag(int fd, int flag)
{
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1)
    {
        perror("fcntl");
        return;
    }

    flags = flags | flag;
    int ret = fcntl(fd, F_SETFL, flags);
    if (ret == -1)
    {
        perror("fcntl");
        return;
    }
}



int main()
{
    int fd = open("a.txt", O_RDWR);
    if (fd == -1) 
    {
        perror("fcntl");
        return -1;
    }

    showflag(fd);
    addflag(fd, O_APPEND);
    showflag(fd);

    close(fd);

    return 0;
}

