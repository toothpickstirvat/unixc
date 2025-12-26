#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>



int main() 
{
    int fd = open("a.dat", O_RDWR|O_CREAT|O_APPEND, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    printf("fd = %d\n", fd);

    printf("write a number\n");
    int n = 1024;
    write(fd, &n, sizeof(n));
    printf("current offset=%ld\n", lseek(fd, 0, SEEK_CUR));

    getchar();
    printf("write a number\n");
    write(fd, &n, sizeof(n));
    printf("offset=%ld\n", lseek(fd, 0, SEEK_CUR));

    getchar();
    printf("set offset to head: %ld\n", lseek(fd, 0, SEEK_SET));

    getchar();
    printf("file size=%ld\n", lseek(fd, 0, SEEK_END));

    return 0;
}
