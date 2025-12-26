#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int fd = open("24lseek.c", O_WRONLY|O_APPEND);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    //write(fd, &fd, sizeof(fd));
    // lseek将文件位置记录在内核中并不引发任何IO操作
    off_t off = lseek(fd, 0, SEEK_CUR);
    printf("%ld\n", off);
    
    
    close(fd);

    return 0;
}
