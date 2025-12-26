#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


int main()
{ 
    int fd = open("a.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }
        
    // 把fd复制成1，1已经打开了，要先关闭
    // 1就指向了原来fd打开的文件（把stdout重定向到了文件）
    int newfd = dup2(fd, 1); //指定一个新的fd指向oldfd打开的文件
    if (newfd == -1) 
    {
        perror("dup2");
        return -1;
    }

    printf("newfd = %d\n", newfd);
    printf("hello world\n");
    int a = 10, b = 20;
    printf("a = %d, b = %d\n", a, b);
    
    // 刷新缓冲区
    fflush(stdout);

    close(newfd);
    close(fd);

    return 0;
}
