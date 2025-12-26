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

    struct flock lock = {};
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; // 锁到文件末尾

    for (int i = 0; i < 1000000; i++)
    {
        lock.l_type = F_WRLCK;
        int ret = fcntl(fd, F_SETLKW, &lock);
        if (ret == -1)
        {
            perror("fcntl lock");
            return -1;
        }

        lseek(fd, 0, SEEK_SET);
        int num = 0;
        read(fd, &num, 4);
        ++num;
        lseek(fd, 0, SEEK_SET);
        write(fd, &num, 4);
    
        lock.l_type = F_UNLCK;
        ret = fcntl(fd, F_SETLKW, &lock);
        if (ret == -1)
        {
            perror("fcntl unlock");
            return -1;
        }
    }

    lseek(fd, 0, SEEK_SET);
    int num = 0;
    read(fd, &num, 4);
    printf("num=%d\n", num);

    close(fd);

    return 0;
}
