#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


int main()
{
    FILE* fp = fopen("std.dat", "w");
    if (fp == NULL) 
    {
        perror("fopen");
        return -1;
    }

    for(int i = 0; i < 10000000; i++) 
    {
        // 写入到缓冲区，只有当缓冲区满时才会将缓冲区中的数据写入到文件，此时才切换到内核态
        fwrite(&i, sizeof(i), 1, fp);
    }

    fclose(fp);

    return 0;
}