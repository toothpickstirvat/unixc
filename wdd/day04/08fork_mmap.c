#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

int main()
{
    int fd = open("a.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }
    ftruncate(fd, 4096);
    //匿名映射
    //void* p = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    
    void* p = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == NULL)
    {
        perror("mmap");
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {
        strcpy(p, "hello,world!");
        sleep(10);
    }
    else
    {
        sleep(3);
        puts((char*)p);
    }

    munmap(p, 4096);

    return 0;
}
