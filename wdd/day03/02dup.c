#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


int main()
{
    int fd1 = open("a.txt", O_RDWR|O_CREAT, 0644);
    if (fd1 == -1)
    {
        perror("open");
        return -1;
    }
    printf("fd1=%d\n", fd1);
    // 复制fd，选取一个最小未使用的fd（分配一个新的fd指向oldfd打开的文件）
    int fd2 = dup(fd1);
    printf("fd2=%d\n", fd2);

    char s[32] = "hello";
    write(fd2, s, strlen(s));
    printf("fd1 offset: %ld\n", lseek(fd1, 0, SEEK_CUR));
    printf("fd2 offset: %ld\n", lseek(fd2, 0, SEEK_CUR));

    ftruncate(fd1, 100);
    printf("fd1 size: %ld\n", lseek(fd1, 0, SEEK_END));
    printf("fd2 size: %ld\n", lseek(fd2, 0, SEEK_END));

    return 0;
}
