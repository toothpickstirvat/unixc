#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


int encrypt(const char* file)
{
    int fd = open(file, O_RDWR);
    if (fd == -1) 
    {
        perror("open");
        return -1;
    }

    char ch = 0;
    while(read(fd, &ch, 1) > 0)
    {
        ch = ~ch;
        // 写回到原位置
        lseek(fd, -1, SEEK_CUR);
        write(fd, &ch, 1);
    }
    close(fd);
}

int main(int argc, char* argv[])
{
    if (argc < 2) 
    {
        printf("%s file\n", argv[0]);
        return -1;
    }

    encrypt(argv[1]);

    return 0;
}
