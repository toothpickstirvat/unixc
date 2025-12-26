#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


int main() 
{
    int fd1 = open("a.dat", O_RDWR|O_CREAT, 0644);
    if (fd1 == -1) 
    {
        perror("open");
        return -1;
    }
    printf("fd1=%d\n", fd1);
    int fd2 = open("a.dat", O_RDWR);
    if (fd2 == -1) 
    {
        perror("open");
        return -1;
    }
    printf("fd2=%d\n", fd2);


    char s[32] = "nihao";
    write(fd1, s, strlen(s));
    printf("fd1 offset: %ld\n", lseek(fd1, 0, SEEK_CUR));
    printf("fd2 offset: %ld\n", lseek(fd2, 0, SEEK_CUR));

    char buff[32] = {};
    read(fd2, buff, 2);
    printf("fd1 offset: %ld\n", lseek(fd1, 0, SEEK_CUR));
    printf("fd2 offset: %ld\n", lseek(fd2, 0, SEEK_CUR));


    printf("fd1 size: %ld\n", lseek(fd1, 0, SEEK_END));
    printf("fd2 size: %ld\n", lseek(fd2, 0, SEEK_END));

    close(fd1);
    close(fd2);

    return 0;
}
