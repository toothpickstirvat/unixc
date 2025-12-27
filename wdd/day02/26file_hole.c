#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char buf1[] = "abcdefghij";
    char buf2[] = "ABCDEFGHIJ";
    int fd;
    if ((fd=creat("file_hole.txt", 0644))<0)
    {
        perror("creat error");
        return -1;
    }
    if (write(fd, buf1, 10) != 10)
    {
        perror("write");
        return -1;
    }
    if (lseek(fd, 16384, SEEK_SET) == -1)
    {
        perror("lseek");
        return -1;
    }
    if (write(fd, buf2, 10) != 10)
    {
        perror("write");
        return -1;
    }

    return 0;
}


